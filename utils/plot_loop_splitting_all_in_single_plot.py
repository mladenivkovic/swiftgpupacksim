#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse

matplotlib.use("Agg")

from plotting_utils import get_filelist
from resultdata import ResultData


# Plot parameters
params = {
    "axes.labelsize": 12,
    "axes.titlesize": 14,
    "font.size": 12,
    "font.family": "serif",
    "legend.fontsize": 8,
    "xtick.labelsize": 12,
    "ytick.labelsize": 12,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.top": True,
    "ytick.right": True,
    "xtick.major.width": 1.5,
    "ytick.major.width": 1.5,
    "axes.linewidth": 1.5,
    "text.usetex": True,
    #  "figure.subplot.left": 0.045,
    #  "figure.subplot.right": 0.99,
    #  "figure.subplot.bottom": 0.05,
    #  "figure.subplot.top": 0.99,
    "figure.subplot.wspace": 0.0,
    #  "figure.subplot.hspace": 0.12,
}
matplotlib.rcParams.update(params)


parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
Plot results of different loop splitting variants for different particle memory
layouts and different particle accessor variants, plotting as much data as
possible in a single plot.

You will need to select one of the following variants to plot:
- alternate between [packed/not packed] and [flush/no flush]
- alternate between [packed/not packed] and [vector/no vector]
- alternate between [flush/no flush] and [vector/no vector]

This script assumes that all subdirectories in the given `srcdir` will contain
result data with filenames 'results_*.csv' It will read them all in and plot
them.

Sub-directories are expected to have the following file name format:
<EXPERIMENTNAME>_<NRTHREADS>threads_<PART_ACCESS>_<LOOP_SPLIT>[_packed][_noflush][_novector]

The *_packed variants shoudl contain outputs where the structs were packed,
i.e. where the code was configured with --enable-packed-structs. The *_noflush
variants should contain outputs where no cache flushes between packing
operations were used (sim ran with --noflush flag). Similarly for *_novector,
where code compiled with --disable-vectorization was used.

Note that the sub-directory sub-strings <EXPERIMENTNAME>, <NRTHREADS>,
<PART_ACCESS>, and <LOOP_SPLIT> are hard-coded in this script. Modify them
manually if you need to.
""",
)
parser.add_argument("srcdir")
parser.add_argument(
    "-v", "--verbose", dest="verbose", action="store_true", help="increase verbosity"
)
parser.add_argument(
    "--flush-vector",
    dest="plot_flush_vector",
    action="store_true",
    help="Plot outputs where caches were flushed/not flushed and those which used manually encouraged vectorization/didn't use it",)
parser.add_argument(
    "--pack-flush",
    dest="plot_pack_flush",
    action="store_true",
    help="Plot outputs where structs were packed/not packed and caches were flushed/not flushed",
)
parser.add_argument(
    "--pack-vector",
    dest="plot_pack_vector",
    action="store_true",
    help="Plot outputs where structs were packed/not packed and those which used manually encouraged vectorization/didn't use it",
)
parser.add_argument(
    "-l",
    "--local",
    dest="local",
    action="store_true",
    help="plot outputs for small local test-runs",
)
parser.add_argument(
    "-p",
    "--png",
    dest="png",
    action="store_true",
    help="make a .png plot instead of .pdf",
)
parser.add_argument(
    "-g",
    "--grace",
    dest="grace",
    action="store_true",
    help="Use default thread numbers for Grace chip (36, 72)",
)
parser.add_argument(
    "-e",
    "--equal-axis-limits",
    dest="equal_axis_limits",
    action="store_true",
    help="Use identical y-axis limits for all subplots",
)

args = parser.parse_args()
srcdir = args.srcdir
verbose = args.verbose
#  use_cache_flush = args.use_cache_flush
local = args.local

EXPERIMENT_NAMES = ["EAGLE12", "Gresho256"]
NRTHREADS = [16, 32]
if args.grace:
    NRTHREADS = [36, 72]

if local:
    EXPERIMENT_NAMES = ["IntelXeonGold5218_Gresho64"]
    NRTHREADS = [4]


multiple_experiments = False
if len(EXPERIMENT_NAMES) > 1:
    multiple_experiments = True

multiple_threads = False
if len(NRTHREADS) > 1:
    multiple_threads = True

LOOP_SPLITS = ["none", "by-struct", "by-element"]
LOOP_SPLIT_LABELS = ["no loop split", "loop split by struct", "loop split by element"]

PART_ACCESS = ["part-struct", "explicit-var", "global-var"]
PART_ACCESS_LABELS = ["part struct access", "explicit var access", "global var access"]


if args.plot_flush_vector and args.plot_pack_vector:
    raise ValueError("Can't plot --pack-vector and --flush-vector simultaneously, pick one.")
if args.plot_flush_vector and args.plot_pack_flush:
    raise ValueError("Can't plot --flush-vector and --pack-flush simultaneously, pick one.")
if args.plot_pack_vector and args.plot_pack_flush:
    raise ValueError("Can't plot --pack-vector and --pack-flush simultaneously, pick one.")
if not (args.plot_flush_vector or args.plot_pack_vector or args.plot_pack_flush):
    raise ValueError("None of --pack-vector, --pack-flush, or --flush-vector selected, I don't know what to plot. Pick one.")

if args.plot_flush_vector:
    dir_suffixes = ["", "_noflush", "_vector", "_noflush_vector"]
    variant_labels = ["", " no flush", " vector", " no flush, vector"]
    plot_variant = "flush-vector"
elif args.plot_pack_vector:
    dir_suffixes = ["", "_packed", "_noflush", "_packed_noflush", ]
    variant_labels = ["", " packed structs", " no flush", " packed structs, no flush"]
    plot_variant = "pack-vector"
elif args.plot_pack_flush:
    dir_suffixes = ["", "_packed",  "_noflush", "_packed_noflush"]
    variant_labels = ["", " packed structs",  " no flush", " packed structs, no flush"]
    plot_variant = "pack-flush"

markers = ["o", "v", "s", "p", "P", "*"]
linestyles = ["-", "--", ":", "-."]


if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    # get available layouts
    layouts = []
    firstdir = (
        EXPERIMENT_NAMES[0]
        + "_"
        + str(NRTHREADS[0])
        + "threads_"
        + PART_ACCESS[0]
        + "_"
        + LOOP_SPLITS[0]
    )
    fullfirstdir = os.path.join(srcdir, firstdir)
    ls = os.listdir(fullfirstdir)
    for f in ls:
        if f.startswith("results_") and f.endswith(".csv"):
            layout = f[len("results_") : -len(".csv")]
            layouts.append(layout)
    #  layouts.sort()

    fig = plt.figure(figsize=(12, 8))
    ax1 = fig.add_subplot(2, 3, 1)
    ax1.set_title("Density/Pack")
    ax2 = fig.add_subplot(2, 3, 2)
    ax2.set_title("Gradient/Pack")
    ax3 = fig.add_subplot(2, 3, 3)
    ax3.set_title("Force/Pack")
    ax4 = fig.add_subplot(2, 3, 4)
    ax4.set_title("Density/Unpack")
    ax5 = fig.add_subplot(2, 3, 5)
    ax5.set_title("Gradient/Unpack")
    ax6 = fig.add_subplot(2, 3, 6)
    ax6.set_title("Force/Unpack")

    maxtime = -1.0
    mintime = 1e32

    index = 0
    for i, name in enumerate(EXPERIMENT_NAMES):
        for j, nthreads in enumerate(NRTHREADS):

            marker = markers[i]

            for a, access in enumerate(PART_ACCESS):
                for s, split in enumerate(LOOP_SPLITS):

                    index += 1
                    color = "C" + str(index)

                    for v, variant in enumerate(dir_suffixes):

                        ls = linestyles[v]
                        result_data = []

                        for l, layout in enumerate(layouts):
                            dirname = (
                                name
                                + "_"
                                + str(nthreads)
                                + "threads_"
                                + access + "_" + split + variant
                            )

                            fulldirname = os.path.join(srcdir, dirname)
                            if not os.path.exists(fulldirname):
                                raise FileNotFoundError(
                                    f"Experiment output directory {fulldirname} not found."
                                )

                            fname = "results_" + layout + ".csv"
                            fullfname = os.path.join(fulldirname, fname)
                            res = ResultData(fullfname, verbose=verbose)
                            result_data.append(res)

                            maxtime = max(maxtime, res.timings.max())
                            mintime = min(mintime, res.timings.min())


                        dens_pack = [res.data_dict["pack/density"] for res in result_data]
                        dens_unpack = [
                            res.data_dict["unpack/density"] for res in result_data
                        ]
                        grad_pack = [res.data_dict["pack/gradient"] for res in result_data]
                        grad_unpack = [
                            res.data_dict["unpack/gradient"] for res in result_data
                        ]
                        forc_pack = [res.data_dict["pack/force"] for res in result_data]
                        forc_unpack = [res.data_dict["unpack/force"] for res in result_data]

                        label = PART_ACCESS_LABELS[a] + " " + LOOP_SPLIT_LABELS[s]
                        if multiple_experiments:
                            label += f" {name}"
                        if multiple_threads:
                            label += f" {nthreads} threads"
                        label += variant_labels[v]

                        plotkwargs = {
                            "label": label,
                            "c": color,
                            "marker": marker,
                            "ls": ls,
                            "lw": 2,
                            "alpha": 0.8,
                            "markersize": 5,
                        }
                        ax1.plot(layouts, dens_pack, **plotkwargs)
                        ax2.plot(layouts, grad_pack, **plotkwargs)
                        ax3.plot(layouts, forc_pack, **plotkwargs)
                        ax4.plot(layouts, dens_unpack, **plotkwargs)
                        ax5.plot(layouts, grad_unpack, **plotkwargs)
                        ax6.plot(layouts, forc_unpack, **plotkwargs)

    #  if mintime < 200.0:
    #      mintime = 0.0

    # all axes
    for ax in fig.axes:
        ax.set_xlabel("particle data layouts")
        ax.tick_params("x", rotation=45)
        ax.grid()
        #  ax.legend()
        if args.equal_axis_limits:
            ax.set_ylim(0.9 * mintime, 1.1 * maxtime)

    # leftmost axes
    for ax in [ax1, ax4]:
        ax.set_ylabel("Timing [ms]")

    # the others
    for ax in [ax2, ax3, ax5, ax6]:
        if args.equal_axis_limits:
            ax.set_yticklabels([])

    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 3
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=2.5,
        markerscale=0.5,
    )
    fig.tight_layout(w_pad=0, rect=(0.01, 0.25, 0.99, 0.99))

    # construct output file name
    outfname = f"loop_splitting_all_{srcdir}_" + plot_variant
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=300)
    print(f"Saved {outfname}")
