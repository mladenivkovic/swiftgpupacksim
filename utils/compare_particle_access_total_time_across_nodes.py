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
Compare total times of using different access variants for different
particle memory layouts for different experiments, for a given loop
splitting variant.

By default, this plots the outputs using:
- cache flushes between each copy operation
- no manual vectorization
- not packed structs
- the runs using 72 threads

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
    "-n",
    "--normalise",
    dest="normalise",
    action="store_true",
    help="Normalise times using t(aos, explicit-var, no loop split)",
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

args = parser.parse_args()
#  nthreads = args.nthreads
#  if isinstance(nthreads, list):
#      nthreads = nthreads[0]
normalise = args.normalise
loop_split = args.loop_split
#  layouts = LAYOUTS_TO_USE_MINIMAL
layouts = LAYOUTS_TO_USE
local = args.local_legion or args.local_hp

if local:
    raise NotImplementedError

#  if args.local_hp:
#      nthreads = 4
#      EXPERIMENTS = ["IntelXeonGold5218_Gresho64"]
#  if args.local_legion:
#      nthreads = 6
#      EXPERIMENTS = ["IntelCoffeeLake_Gresho128"]


variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

plotkwargs = {
    #  "marker": "o",
    "lw": 2,
    "alpha": 0.8,
    "markersize": 5,
}


if __name__ == "__main__":

    fig = plt.figure(figsize=(10, 3))
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

                color = "C" + str(a)

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

                results = np.array(result_data)

                if normalise:
                    results /= normalisation

                label = (
                        experiment + " " +
                    PART_ACCESS_LABELS[a]
                    #  + " "
                    #  + LOOP_SPLIT_LABELS[s]
                    #  + variant_label_suffix
                )

                ax.plot(layouts, results, c=color, ls=ls, label=label, marker=marker,**plotkwargs)

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
        ax.set_ylim(0.60, 1.3)

        if normalise:
            ax.set_ylabel(
                r"$t / t^{\mathrm{part-struct}}_{\mathrm{aos}}$"
            )
        else:
            ax.set_ylabel("Timing [ms]")

        # the others
        #  for ax in [ax2, ax3, ax5, ax6]:
        #      if args.equal_axis_limits:
        #          ax.set_yticklabels([])


    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 3
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=2.5,
        markerscale=1.,
        fontsize="medium",
    )
    fig.tight_layout(w_pad=1, rect=(0.01, 0.16, 0.99, 0.99))

    # construct output file name
    outfname = f"compare_part_access_total_across_nodes"
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
