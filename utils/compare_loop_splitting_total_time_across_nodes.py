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
    NODES,
    NODE_LABELS,
    mymplparams,
    mydpi,
    markers,
    linestyles,
)


matplotlib.rcParams.update(mymplparams)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
    TODO
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
parser.add_argument(
    "-a",
    "--access",
    nargs=1,
    dest="access_variant",
    help="Particle access variant to use",
    choices=PART_ACCESS,
    default="explicit-var",
    type=str,
)

args = parser.parse_args()
# nthreads = args.nthreads
local = args.local_legion or args.local_hp
access_variant=args.access_variant
if isinstance(access_variant, list):
    access_variant = access_variant[0]

if args.equal_axis_limits:
    raise NotImplementedError()
if args.local_hp or args.local_legion:
    raise ValueError("Plotting this doesn't make sense at this point.")
if local:
    raise NotImplementedError


variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

plotkwargs = {
    #  "marker": "o",
    "lw": 2,
    "alpha": 0.6,
    "markersize": 5,
}

layouts = LAYOUTS_TO_USE

if __name__ == "__main__":

    fig = plt.figure(figsize=(10, 4))
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

        if srcdir.startswith("dine2"):
            nthreads = 64
        elif srcdir.startswith("gn003"):
            nthreads = 72
        else:
            raise NotImplementedError

        for e, experiment in enumerate(EXPERIMENTS):

            ls = linestyles[e]
            marker=markers[e]

            # first, grab normalisation:
            # AoS part-struct for this experiment
            normfname = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        access_variant,
                        "none",
                        variant_dir_suffix,
                        "aos",
                    )
            res = ResultData(normfname, verbose=False)
            normalisation = res.total_time

            for s, split in enumerate(LOOP_SPLITS):
                color = "C" + str(s)

                # Get result data for all layouts
                result_data = []

                for l, layout in enumerate(layouts):

                    fname = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        access_variant,
                        split,
                        variant_dir_suffix,
                        layout,
                    )
                    res = ResultData(fname, verbose=False)
                    result_data.append(res.total_time)

                    #  maxtime = max(maxtime, res.timings.max())
                    #  mintime = min(mintime, res.timings.min())

                results = np.array(result_data)
                results /= normalisation

                label = (
                    experiment
                    #  + " "
                    #  + PART_ACCESS_LABELS[a]
                    + " "
                    + LOOP_SPLIT_LABELS[s]
                    #  + variant_label_suffix
                )

                ax.plot(layouts, results, c=color, ls=ls, label=label, marker=marker,**plotkwargs)

    #  if mintime < 200.0:
    #      mintime = 0.0

    # all axes
    for ax in fig.axes:
        #  ax.set_xlabel("particle data layouts")
        #  ax.set_xticks(ax.get_xticks(), labels=ax.get_xticklabels(), rotation=30, ha="right", rotation_mode="anchor" )
        ax.set_xticks(ax.get_xticks(), labels=ax.get_xticklabels(), rotation=30, ha="right", rotation_mode="anchor" )
        ax.grid(which="both")
        #  ax.legend()
        #  if args.equal_axis_limits:
        #      ax.set_ylim(0.9 * mintime, 1.1 * maxtime)

        ax.set_ylabel(r"$t / t^{\mathrm{none}}_{\mathrm{aos}}$")


    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 3
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=4.5,
        markerscale=1.,
    )
    fig.tight_layout(w_pad=0.5, rect=(0.01, 0.16, 0.99, 0.99))

    # construct output file name
    outfname = f"compare_loop_splitting_total_times_across_nodes_{access_variant}"
    if variant_dir_suffix != "":
        outfname += variant_dir_suffix
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
    plt.close()
