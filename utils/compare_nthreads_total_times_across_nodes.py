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
    NODE_LABELS,
    LAYOUTS_TO_USE,
    LAYOUTS_TO_USE_MINIMAL,
    mymplparams,
    mydpi,
    markers,
    linestyles,
)

matplotlib.rcParams.update(mymplparams)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
    TODO: WRITE CORRECT DESCRIPTION

By default, this plots the outputs using:
- cache flushes between each copy operation
- no manual vectorization
- not packed structs

See optional flags to modify which results are plotted.

This script assumes that all subdirectories in the hardcoded `NODES` will
contain result data with filenames 'results_*.csv' It will read them all in and
plot them.

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
normalise = args.normalise
access_variant = args.access_variant
loop_split = args.loop_split
local = args.local_legion or args.local_hp

if args.equal_axis_limits:
    raise NotImplementedError()

if args.local_hp:
    raise NotImplementedError()
    #  NTHREADS = [4]
    #  EXPERIMENTS = ["IntelXeonGold5218_Gresho64"]
if args.local_legion:
    raise NotImplementedError()
    #  NTHREADS = [6]
    #  EXPERIMENTS = ["IntelCoffeeLake_Gresho128"]

NODES = ["dine2", "gn003"]

variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

plotkwargs = {
    "marker": "o",
    "lw": 2,
    "alpha": 0.8,
    "markersize": 5,
}

#  layouts = LAYOUTS_TO_USE
layouts = LAYOUTS_TO_USE_MINIMAL

if __name__ == "__main__":

    aos_ind = layouts.index("aos")

    fig = plt.figure(figsize=(12, 6))
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)
    axes = [ax1, ax2]

    maxtime = -1.0
    mintime = 1e32

    for n,srcdir in enumerate(NODES):

        ax = axes[n]
        ax.set_title(NODE_LABELS[srcdir])

        if srcdir.endswith("gn003") or srcdir.endswith("gn003/"):
            NTHREADS=[1, 9, 18, 36, 72]
        elif srcdir.endswith("dine2") or srcdir.endswith("dine2/"):
            NTHREADS=[1, 4, 8, 16, 32, 64]
        elif srcdir.endswith("mad06") or srcdir.endswith("mad06/"):
            NTHREADS=[4, 8, 16, 32, 64, 128]


        for e, experiment in enumerate(EXPERIMENTS):

            ls = linestyles[e]

            # first, grab normalisation:
            # AoS on single thread for this experiment
            fname_norm = get_result_fname(
                        srcdir,
                        experiment,
                        1,
                        access_variant,
                        loop_split,
                        variant_dir_suffix,
                        "aos",
                    )
            res = ResultData(fname_norm, verbose=False)
            normalisation = res.total_time



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
                    result_data.append(res.total_time)

                    #  maxtime = max(maxtime, res.timings.max())
                    #  mintime = min(mintime, res.timings.min())

                results = np.array(result_data)

                if normalise:
                    results /= normalisation

                label = (
                        #  NODE_LABELS[srcdir] + " " +
                        experiment + " "
                    #  PART_ACCESS_LABELS[PART_ACCESS.index(access_variant)]
                    #  + " "
                    #  + LOOP_SPLIT_LABELS[LOOP_SPLITS.index(loop_split)]
                    #  + variant_label_suffix
                    #  + " "
                    + str(nthreads)
                    + " threads"
                )

                ax.plot(
                    layouts, results, c=color, ls=ls, label=label, **plotkwargs
                )

    #  if mintime < 200.0:
    #      mintime = 0.0

    # all axes
    for ax in fig.axes:
        ax.set_xlabel("particle data layouts")
        #  ax.tick_params("x", rotation=45)
        ax.set_xticks(ax.get_xticks(), labels=ax.get_xticklabels(), rotation=30, ha="right", rotation_mode="anchor" )
        ax.grid()
        #  ax.legend()
        #  if args.equal_axis_limits:
        #      ax.set_ylim(0.9 * mintime, 1.1 * maxtime)

        if normalise:
            ax.set_ylabel( r"$t / t_{\mathrm{aos}}$")
        else:
            ax.set_ylabel("Timing [ms]")

    # top row axes
    #  for ax in [ax1, ax2, ax3]:
    #      ax.set_xticklabels([])
    #      ax.set_xlabel(None)

    # the others
    #  for ax in [ax2, ax3, ax5, ax6]:
    #      if args.equal_axis_limits:
    #          ax.set_yticklabels([])

    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    #  ncols = 2
    ncols = 4
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
    outfname = f"compare_nthreads_total_times_{access_variant}_{loop_split}"
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
