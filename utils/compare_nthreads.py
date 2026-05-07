#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse
import numpy as np

matplotlib.use("Agg")

from resultdata import ResultData
from plotting_utils import (
    get_filelist,
    get_variant_labels,
    get_result_fname,
    get_result_dir,
    LOOP_SPLITS,
    LOOP_SPLIT_LABELS,
    PART_ACCESS,
    PART_ACCESS_LABELS,
    EXPERIMENTS,
    NTHREADS,
    mymplparams,
    mydpi,
    markers,
    linestyles,
)

matplotlib.rcParams.update(mymplparams)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
Compare results of using different numbers of threads for different
particle memory layouts for different experiments, for a given loop
splitting variant, and a given particle accessor variant.

By default, this plots the outputs using:
- cache flushes between each copy operation
- no manual vectorization
- not packed structs

See optional flags to modify which results are plotted.

This script assumes that all subdirectories in the given `srcdir` will contain
result data with filenames 'results_*.csv' It will read them all in and plot
them.

Sub-directories are expected to have the following file name format:
<EXPERIMENTNAME>_<NRTHREADS>threads_<PART_ACCESS>_<LOOP_SPLIT>[_packed][_noflush][_vector]

The *_packed variants should contain outputs where the structs were packed,
i.e. where the code was configured with --enable-packed-structs. The *_noflush
variants should contain outputs where no cache flushes between packing
operations were used (sim ran with --noflush flag). Similarly for *_vector,
where code compiled with --enable-manual-vectorization was used.

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
    help="Compare to outputs where caches were NOT flushed",
)
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
parser.add_argument(
    "-a",
    "--access",
    nargs=1,
    dest="access_variant",
    help="Particle access variant to use",
    choices=PART_ACCESS,
    default="explicit-var",
)
parser.add_argument(
    "-l",
    "--loop",
    nargs=1,
    dest="loop_split",
    help="Loop splitting variant to use",
    choices=LOOP_SPLITS,
    default="none",
)
parser.add_argument(
    "-n",
    "--normalise",
    dest="normalise",
    action="store_true",
    help="Normalise times using t(aos, no loop split)",
)
parser.add_argument(
    "--local-legion",
    dest="local_legion",
    action="store_true",
    help="Use outputs for local test runs on lenovo legion",
)
parser.add_argument(
    "--local-hp",
    dest="local_hp",
    action="store_true",
    help="Use outputs for local test runs on HP",
)

args = parser.parse_args()
srcdir = args.srcdir
normalise = args.normalise
access_variant = args.access_variant
loop_split = args.loop_split
local = args.local_legion or args.local_hp

if normalise:
    raise NotImplementedError()
if args.equal_axis_limits:
    raise NotImplementedError()

if srcdir.endswith("gn003") or srcdir.endswith("gn003/"):
    NTHREADS=[1, 9, 18, 36, 72]
elif srcdir.endswith("dine2") or srcdir.endswith("dine2/"):
    NTHREADS=[4, 8, 16, 32, 64]
elif srcdir.endswith("mad06") or srcdir.endswith("mad06/"):
    NTHREADS=[4, 8, 16, 32, 64, 128]

if args.local_hp:
    NTHREADS = [4]
    EXPERIMENTS = ["IntelXeonGold5218_Gresho64"]
if args.local_legion:
    NTHREADS = [6]
    EXPERIMENTS = ["IntelCoffeeLake_Gresho128"]


variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

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
    firstdir = get_result_dir(
        srcdir, EXPERIMENTS[0], NTHREADS[-1], access_variant, loop_split, variant_dir_suffix
        )
    ls = os.listdir(firstdir)
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
        raise ValueError(
            "Something went wrong determining index of AoS in array,", layouts
        )

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

        # first, get normalisation: Compare to access="part-struct", loop-split = "none"
        #  dirname = ( experiment + "_" + str(nthreads) + "threads_part-struct_none" + variant_dir_suffix)
        #  fulldirname = os.path.join(srcdir, dirname)
        #  if not os.path.exists(fulldirname):
        #      raise FileNotFoundError(
        #          f"Experiment output directory {fulldirname} not found."
        #      )
        #
        #  fname = "results_aos.csv"
        #  fullfname = os.path.join(fulldirname, fname)
        #  res = ResultData(fullfname, verbose=False)
        #  normalisation = res.data_dict

        for n, nthreads in enumerate(NTHREADS):

            color = "C" + str(n)

            # Now get result data for all layouts
            result_data = []

            for l, layout in enumerate(layouts):
                fname = get_result_fname(
                    srcdir,
                    experiment,
                    nthreads,
                    access_variant,
                    loop_split,
                    variant_dir_suffix,
                    layout,
                )
                res = ResultData(fname, verbose=False)
                result_data.append(res)

                maxtime = max(maxtime, res.timings.max())
                mintime = min(mintime, res.timings.min())

            # Unpack result data by packing operation type
            dens_pack = np.array(
                [res.data_dict["pack/density"] for res in result_data]
            )
            dens_unpack = np.array(
                [res.data_dict["unpack/density"] for res in result_data]
            )
            grad_pack = np.array(
                [res.data_dict["pack/gradient"] for res in result_data]
            )
            grad_unpack = np.array(
                [res.data_dict["unpack/gradient"] for res in result_data]
            )
            forc_pack = np.array(
                [res.data_dict["pack/force"] for res in result_data]
            )
            forc_unpack = np.array(
                [res.data_dict["unpack/force"] for res in result_data]
            )

            #  if normalise:
            #      dens_pack /= normalisation["pack/density"]
            #      dens_unpack /= normalisation["unpack/density"]
            #      grad_pack /= normalisation["pack/gradient"]
            #      grad_unpack /= normalisation["unpack/gradient"]
            #      forc_pack /= normalisation["pack/force"]
            #      forc_unpack /= normalisation["unpack/force"]

            label = (
                    experiment + " " +
                PART_ACCESS_LABELS[PART_ACCESS.index(access_variant)]
                + " "
                + LOOP_SPLIT_LABELS[LOOP_SPLITS.index(loop_split)]
                + variant_label_suffix
                + " "
                + str(nthreads)
                + " threads"
            )

            ax1.plot(
                layouts, dens_pack, c=color, ls=ls, label=label, **plotkwargs
            )
            ax2.plot(
                layouts, grad_pack, c=color, ls=ls, label=label, **plotkwargs
            )
            ax3.plot(
                layouts, forc_pack, c=color, ls=ls, label=label, **plotkwargs
            )
            ax4.plot(
                layouts, dens_unpack, c=color, ls=ls, label=label, **plotkwargs
            )
            ax5.plot(
                layouts, grad_unpack, c=color, ls=ls, label=label, **plotkwargs
            )
            ax6.plot(
                layouts, forc_unpack, c=color, ls=ls, label=label, **plotkwargs
            )

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
        if normalise:
            ax.set_ylabel(
                r"$t / t^{\mathrm{part\ struct\ access}}_{\mathrm{aos,\ no\ loop\ split}}$"
            )
        else:
            ax.set_ylabel("Timing [ms]")

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
        handlelength=2.5,
        markerscale=0.5,
    )
    fig.tight_layout(w_pad=0, rect=(0.01, 0.12, 0.99, 0.99))

    # construct output file name
    outfname = f"compare_nthreads_{srcdir}_{access_variant}"
    if variant_dir_suffix != "":
        outfname += variant_dir_suffix
    if normalise:
        outfname += "_normalised"
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
    plt.close()
