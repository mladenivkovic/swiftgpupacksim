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
    Compare results of the "cellsize tests" for different nodes
    for a given loop splitting method.
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
#  parser.add_argument(
#      "-t",
#      "--threads",
#      nargs=1,
#      dest="nthreads",
#      help="Number of threads used in experiment to use",
#      default=72,
#  )
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
#  parser.add_argument(
#      "-a",
#      "--access",
#      nargs=1,
#      dest="access_variant",
#      help="Particle access variant to use",
#      choices=PART_ACCESS,
#      default="explicit-var",
#  )
parser.add_argument(
    "-l",
    "--loop",
    nargs=1,
    dest="loop_split",
    help="Loop splitting variant to use",
    choices=LOOP_SPLITS,
    default="none",
    type=str,
)

args = parser.parse_args()
loop_split = args.loop_split
#  layouts = LAYOUTS_TO_USE_MINIMAL
layouts = LAYOUTS_TO_USE

variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

plotkwargs = {
    "marker": "o",
    "lw": 2,
    "alpha": 0.8,
    "markersize": 5,
}

NODES = ["dine2_cellsize", "gn003_cellsize"]
NODE_LABELS = {"dine2_cellsize": "dine2", "gn003_cellsize":"gracehopper"}

EXPERIMENTS = [
    "TestCellSize_64",
    "TestCellSize_128",
    "TestCellSize_512",
    #  "TestCellSize_1024",
    "TestCellSize_2048",
    ]
EXPERIMENT_LABLES = [
        "N=64", "N=128", "N=512",
        #  "N=1024",
        "N=2048"
        ]

nthreads = -1



if __name__ == "__main__":

    fig = plt.figure(figsize=(11, 4))
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)
    axes = [ax1, ax2]

    #  maxtime = -1.0
    #  mintime = 1e32

    for n, srcdir in enumerate(NODES):

        if not os.path.exists(srcdir):
            raise FileNotFoundError(f"directory {srcdir} not found.")

        ax = axes[n]
        ax.set_title(NODE_LABELS[srcdir])

        for e, experiment in enumerate(EXPERIMENTS):

            color = "C" + str(e)

            # first, grab normalisation:
            # AoS part-struct for this experiment
            fname_norm = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        "part-struct",
                        loop_split,
                        variant_dir_suffix,
                        "aos",
                    )
            res = ResultData(fname_norm, verbose=False)
            normalisation = res.total_time

            for a, access in enumerate(PART_ACCESS):

                ls = linestyles[a]

                # Now get result data for all layouts
                result_data = []

                for l, layout in enumerate(layouts):

                    fname = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        access,
                        loop_split,
                        variant_dir_suffix,
                        layout,
                    )
                    res = ResultData(fname, verbose=False)
                    result_data.append(res.total_time)

                    #  maxtime = max(maxtime, res.timings.max())
                    #  mintime = min(mintime, res.timings.min())


                # Unpack result data by packing operation type
                results = np.array(result_data)

                results /= normalisation

                label = (
                        EXPERIMENT_LABLES[e] + " " +
                    PART_ACCESS_LABELS[a]
                    #  + " "
                    #  + LOOP_SPLIT_LABELS[s]
                    #  + variant_label_suffix
                )

                ax.plot(layouts, results, c=color, ls=ls, label=label, **plotkwargs)

    #  if mintime < 200.0:
    #      mintime = 0.0

    for ax in axes:
        #  ax.set_xlabel("particle data layouts")
        #  ax.tick_params("x", rotation=90)
        ax.set_xticks(ax.get_xticks(), labels=ax.get_xticklabels(), rotation=30, ha="right", rotation_mode="anchor" )
        ax.grid(which="both")
        #  ax.legend()
        #  if args.equal_axis_limits:
        #      ax.set_ylim(0.9 * mintime, 1.1 * maxtime)
        ax.set_ylim(0.50, 1.3)

        # leftmost axes
        ax.set_ylabel(
            r"$t / t_{\mathrm{aos}}$"
        )
        #  else:
        #      ax.set_ylabel("Timing [ms]")

        # the others
        #  for ax in [ax2, ax3, ax5, ax6]:
        #      if args.equal_axis_limits:
        #          ax.set_yticklabels([])


    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 4
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=2.5,
        markerscale=0.5,
        fontsize="medium",
    )
    fig.tight_layout(w_pad=1, rect=(0.01, 0.16, 0.99, 0.99))

    # construct output file name
    outfname = f"compare_cellsize_across_nodes"
    if variant_dir_suffix != "":
        outfname += variant_dir_suffix
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
