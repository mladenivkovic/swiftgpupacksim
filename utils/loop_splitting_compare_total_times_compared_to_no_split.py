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
    LAYOUTS_TO_USE,
    NODE_LABELS,
    EXPERIMENTS,
    mymplparams,
    mydpi,
    markers,
    linestyles,
)

PART_ACCESS = ["part-struct", "explicit-var"]

matplotlib.rcParams.update(mymplparams)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
    TODO
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
nthreads = args.nthreads
if isinstance(nthreads, list):
    nthreads = nthreads[0]
local = args.local_legion or args.local_hp

if args.equal_axis_limits:
    raise NotImplementedError()
if args.local_hp or args.local_legion:
    raise ValueError("Plotting this doesn't make sense at this point.")


variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

plotkwargs = {
    #  "marker": "o",
    "lw": 2,
    "alpha": 0.6,
    "markersize": 5,
}

if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    # get available layouts
    layouts = LAYOUTS_TO_USE
#      firstdir = get_result_dir(
    #      srcdir, EXPERIMENTS[0], nthreads, PART_ACCESS[0], LOOP_SPLITS[0]
    #  )
    #  ls = os.listdir(firstdir)
    #  for f in ls:
    #      if f.startswith("results_") and f.endswith(".csv"):
    #          layout = f[len("results_") : -len(".csv")]
    #          layouts.append(layout)
#      layouts.sort()

    if "dine2" in srcdir:
        fig = plt.figure(figsize=(12, 3))
    else:
        fig = plt.figure(figsize=(12, 3.5))
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)
    axes = [ax1, ax2]

    #  maxtime = -1.0
    #  mintime = 1e32

    for e, experiment in enumerate(EXPERIMENTS):
        ax = axes[e]
        ax.set_title(experiment)


        for a, part_access in enumerate(PART_ACCESS):

            ls = linestyles[a]
            marker = markers[a]


            for s, split in enumerate(LOOP_SPLITS):

                color = "C" + str(s)

                # Get result data for all layouts
                result_data = []

                for l, layout in enumerate(layouts):

                    # first, get the normalisation
                    # part-struct no-loop-split for this experiment for this layout
                    fname_norm = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        part_access,
                        "none",
                        variant_dir_suffix,
                        layout,
                    )
                    res = ResultData(fname_norm, verbose=False)
                    normalisation = res.total_time


                    fname = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        part_access,
                        split,
                        variant_dir_suffix,
                        layout,
                    )
                    res = ResultData(fname, verbose=False)
                    result_data.append(res.total_time / normalisation)

                    #  maxtime = max(maxtime, res.timings.max())
                    #  mintime = min(mintime, res.timings.min())

                results = np.array(result_data)
                label = (
                    #  experiment
                    #  + " " +
                    PART_ACCESS_LABELS[a]
                    + ", "
                    + LOOP_SPLIT_LABELS[s]
                    #  + variant_label_suffix
                )

                ax.plot(
                    layouts,
                    results,
                    c=color,
                    ls=ls,
                    marker=marker,
                    label=label,
                    **plotkwargs,
                )

    #  if mintime < 200.0:
    #      mintime = 0.0

    # all axes
    for ax in fig.axes:
        #  ax.set_xlabel("particle data layouts")
        #  ax.tick_params("x", rotation=90)
        ax.set_xticks(ax.get_xticks(), labels=ax.get_xticklabels(), rotation=30, ha="right", rotation_mode="anchor" )
        ax.grid(which="both")
        #  ax.legend()
        if args.equal_axis_limits:
            ax.set_ylim(0.9 * mintime, 1.1 * maxtime)

        ax.set_ylabel(
            r"$t / t_{\mathrm{by-particle}}$"#^{\mathrm{part-struct}}$"
            #  r"$t / t_{\mathrm{\ no\ loop\ split}}$"#^{\mathrm{part-struct}}$"
        )
        if NODE_LABELS[srcdir] == "gracehopper":
            ax.annotate(NODE_LABELS[srcdir], xy=(0.03, 0.83), xycoords='axes fraction', backgroundcolor="lightgrey", fontsize="large")
        else:
            ax.annotate(NODE_LABELS[srcdir], xy=(0.83, 0.83), xycoords='axes fraction', backgroundcolor="lightgrey", fontsize="large", ha="center")

        if NODE_LABELS[srcdir] == "gracehopper":
            ax.set_yticks([0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5])

    # the others
        #  if args.equal_axis_limits:
        #      ax.set_yticklabels([])

    if "dine2" in srcdir:
        fig.tight_layout(w_pad=0.7, rect=(0.0, 0.0, 1.0, 1.0))
    else:
        hand, lab = ax1.get_legend_handles_labels()
        #  ncols=int(len(layouts)*0.5 + 0.5)
        ncols = 3
        fig.legend(
            handles=hand,
            labels=lab,
            loc="lower center",
            ncols=ncols,
            handlelength=4.5,
            markerscale=1.0,
        )
        #  fig.suptitle(NODE_LABELS[srcdir])
        fig.tight_layout(w_pad=0.5, rect=(0.0, 0.15, 1.0, 1.0))

    # construct output file name
    outfname = f"loop_splitting_compare_relative_total_time_{srcdir}_{nthreads}threads"
    if variant_dir_suffix != "":
        outfname += variant_dir_suffix
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
    plt.close()
