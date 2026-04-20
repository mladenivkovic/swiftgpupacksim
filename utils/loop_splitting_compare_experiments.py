#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse
import numpy as np

matplotlib.use("Agg")

from plotting_utils import get_filelist, LOOP_SPLITS, LOOP_SPLIT_LABELS, PART_ACCESS, PART_ACCESS_LABELS
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
layouts and different experiments.

For an adequate comparable plot, the results are normalized using the times for
layout = aos

By default, this plots the outputs using:
- cache flushes between each copy operation
- no manual vectorization
- not packed structs
- the runs using 72 threads
- the `explicit-var` particle access method.

See optional flags to modify which results are plotted.

This script assumes that all subdirectories in the given `srcdir` will contain
result data with filenames 'results_*.csv' It will read them all in and plot
them.

Sub-directories are expected to have the following file name format:
<EXPERIMENTNAME>_<NRTHREADS>threads_<PART_ACCESS>_<LOOP_SPLIT>[_packed][_noflush][_novector]

The *_packed variants should contain outputs where the structs were packed,
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
        "-f",
    "--no-flush",
    dest="use_noflush",
    action="store_true",
    help="Compare to outputs where caches were NOT flushed",)
parser.add_argument(
        "-v",
    "--vector",
    dest="use_vector",
    action="store_true",
    help="Compare to outputs where SIMD vectorization was enabled",
)
parser.add_argument(
        "-p",
    "--packed",
    dest="use_packed",
    action="store_true",
    help="Compare to outputs where structs were packed",
)
parser.add_argument(
        "-t",
    "--threads",
    nargs=1,
    dest="nthreads",
    help="Number of threads used in experiment to use",
    default=72,
)
parser.add_argument(
    "--png",
    dest="png",
    action="store_true",
    help="make a .png plot instead of .pdf",
)
parser.add_argument(
    "-e",
    "--equal-axis-limits",
    dest="equal_axis_limits",
    action="store_true",
    help="Use identical y-axis limits for all subplots",
)
# TODO: Add options for other particle access variants.

args = parser.parse_args()
srcdir = args.srcdir
nthreads = args.nthreads

EXPERIMENTS = ["Gresho256", "EAGLE12"]

part_access = "explicit-var"
part_access_label = "explicit var access"

variant_dir_suffix = ""
variant_label_suffix = ""
if args.use_noflush:
    variant_dir_suffix += "_noflush"
    variant_label_suffix += ", no flush"
if args.use_vector:
    variant_dir_suffix += "_novector"
    variant_label_suffix += ", no vector"
if args.use_packed:
    variant_dir_suffix += "_packed"
    variant_label_suffix += ", packed structs"


markers = ["o", "v", "s", "p", "P", "*"]
linestyles = ["-", "--", ":", "-."]

plotkwargs = {
    "marker": "o",
    "lw": 2,
    "alpha": 0.8,
    "markersize": 5,
}

if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    # get available layouts
    layouts = []
    firstdir = (
        EXPERIMENTS[0]
        + "_"
        + str(nthreads)
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
    layouts.sort()

    aos_ind = -1
    for i in range(len(layouts)):
        if layouts[i] == "aos":
            aos_ind = i
            break
    if aos_ind == -1:
        raise ValueError("Something went wrong determining index of AoS in array,", layouts)

    fig = plt.figure(figsize=(12, 6))
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

    for e, experiment in enumerate(EXPERIMENTS):
        ls = linestyles[e]

        for s, split in enumerate(LOOP_SPLITS):
            color = "C" + str(s)

            # Get result data for all layouts
            result_data = []

            for l, layout in enumerate(layouts):
                dirname = (
                    experiment
                    + "_"
                    + str(nthreads)
                    + "threads_"
                    + part_access + "_" + split + variant_dir_suffix
                )

                fulldirname = os.path.join(srcdir, dirname)
                if not os.path.exists(fulldirname):
                    raise FileNotFoundError(
                        f"Experiment output directory {fulldirname} not found."
                    )

                fname = "results_" + layout + ".csv"
                fullfname = os.path.join(fulldirname, fname)
                res = ResultData(fullfname, verbose=False)
                result_data.append(res)

                maxtime = max(maxtime, res.timings.max())
                mintime = min(mintime, res.timings.min())

            # Unpack result data by packing operation type
            dens_pack = np.array([res.data_dict["pack/density"] for res in result_data])
            dens_unpack = np.array([ res.data_dict["unpack/density"] for res in result_data ])
            grad_pack = np.array([res.data_dict["pack/gradient"] for res in result_data])
            grad_unpack = np.array([ res.data_dict["unpack/gradient"] for res in result_data ])
            forc_pack = np.array([res.data_dict["pack/force"] for res in result_data])
            forc_unpack = np.array([res.data_dict["unpack/force"] for res in result_data])

            label = experiment + " " + part_access_label + " " + LOOP_SPLIT_LABELS[s] + variant_label_suffix

            ax1.plot(layouts, dens_pack / dens_pack[aos_ind], c=color, ls=ls, label=label, **plotkwargs)
            ax2.plot(layouts, grad_pack / grad_pack[aos_ind], c=color, ls=ls, label=label, **plotkwargs)
            ax3.plot(layouts, forc_pack / forc_pack[aos_ind], c=color, ls=ls, label=label, **plotkwargs)
            ax4.plot(layouts, dens_unpack / dens_unpack[aos_ind], c=color, ls=ls, label=label, **plotkwargs)
            ax5.plot(layouts, grad_unpack / grad_unpack[aos_ind], c=color, ls=ls, label=label, **plotkwargs)
            ax6.plot(layouts, forc_unpack / forc_unpack[aos_ind], c=color, ls=ls, label=label, **plotkwargs)

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
        ax.set_ylabel(r"$t / t_{\mathrm{aos}}$")

    # top row axes
    for ax in [ax1, ax2, ax3]:
        ax.set_xticklabels([])
        ax.set_xlabel(None)

    # the others
    for ax in [ax2, ax3, ax5, ax6]:
        if args.equal_axis_limits:
            ax.set_yticklabels([])

    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 2
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=4.5,
        markerscale=0.75,
    )
    fig.tight_layout(w_pad=0, rect=(0.01, 0.12, 0.99, 0.99))

    # construct output file name
    outfname = f"compare_experiments_loop_splitting_{nthreads}threads"
    if variant_dir_suffix != "":
        outfname += variant_dir_suffix
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=300)
    print(f"Saved {outfname}")
