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
)

matplotlib.rcParams.update(mymplparams)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
Compare times for different loop splitting variants in a heatmap
and identify best loop splitting variant for each interaction type and subtype
for a given particle access method.

For an adequate comparable plot, the results are normalized using the times for
layout = aos and access variant = part-struct.

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
srcdir = args.srcdir
nthreads = args.nthreads
if isinstance(nthreads, list):
    nthreads = nthreads[0]
access_variant = args.access_variant
loop_split = args.loop_split
layouts = LAYOUTS_TO_USE
#  layouts = LAYOUTS_TO_USE_MINIMAL

local = args.local_legion or args.local_hp

if local:
    raise NotImplementedError()
if args.equal_axis_limits:
    raise NotImplementedError()
if args.local_hp or args.local_legion:
    raise ValueError("Plotting this doesn't make sense at this point.")


variant_dir_suffix, variant_label_suffix = get_variant_labels(
    args.use_noflush, args.use_vector, args.use_packed
)

plotkwargs = {
    "marker": "o",
    "lw": 2,
    "alpha": 0.6,
    "markersize": 5,
}


def plot_heatmap(ax, heatmap):

    im = ax.imshow(heatmap, cmap="viridis_r")
    ax.set_xticks(range(len(LOOP_SPLITS)), labels=LOOP_SPLITS, rotation=30, ha="right", rotation_mode="anchor")
    ax.set_yticks(range(len(layouts)), labels=layouts,)

    maxval = heatmap.max()
    mean = heatmap.mean()

    # add text annotations
    for l in range(len(layouts)):
        for s in range(len(LOOP_SPLITS)):
            colour = "black"
            if heatmap[l,s] > 0.5 * (maxval + mean):
                colour = "white"
            text = ax.text(s, l, f"{heatmap[l,s]:.2f}", ha="center", va="center", color=colour)

    return im





if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    for e, experiment in enumerate(EXPERIMENTS):

        fig = plt.figure(figsize=(14, 9))
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

        # get reference value
        reffname = get_result_fname(
            srcdir,
            experiment,
            nthreads,
            "part-struct",
            "none",
            variant_dir_suffix,
            "aos",
        )
        res = ResultData(reffname, verbose=False)
        ref = res.data_dict

        heatmap_dp = np.zeros((len(layouts), len(LOOP_SPLITS)))
        heatmap_du = np.zeros((len(layouts), len(LOOP_SPLITS)))
        heatmap_gp = np.zeros((len(layouts), len(LOOP_SPLITS)))
        heatmap_gu = np.zeros((len(layouts), len(LOOP_SPLITS)))
        heatmap_fp = np.zeros((len(layouts), len(LOOP_SPLITS)))
        heatmap_fu = np.zeros((len(layouts), len(LOOP_SPLITS)))

        for s, split in enumerate(LOOP_SPLITS):

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

                heatmap_dp[l, s] = res.data_dict["pack/density"]
                heatmap_du[l, s] = res.data_dict["unpack/density"]
                heatmap_gp[l, s] = res.data_dict["pack/gradient"]
                heatmap_gu[l, s] = res.data_dict["unpack/gradient"]
                heatmap_fp[l, s] = res.data_dict["pack/force"]
                heatmap_fu[l, s] = res.data_dict["unpack/force"]

                maxtime = max(maxtime, res.timings.max())
                mintime = min(mintime, res.timings.min())

        # apply normalisation
        heatmap_dp /= ref["pack/density"]
        heatmap_du /= ref["unpack/density"]
        heatmap_gp /= ref["pack/gradient"]
        heatmap_gu /= ref["unpack/gradient"]
        heatmap_fp /= ref["pack/force"]
        heatmap_fu /= ref["unpack/force"]

        im1 = plot_heatmap(ax1, heatmap_dp)
        im2 = plot_heatmap(ax2, heatmap_du)
        im3 = plot_heatmap(ax3, heatmap_gp)
        im4 = plot_heatmap(ax4, heatmap_gu)
        im5 = plot_heatmap(ax5, heatmap_fp)
        im6 = plot_heatmap(ax6, heatmap_fu)

        fig.tight_layout(w_pad=0, rect=(0.01, 0.01, 0.99, 0.99))

        # construct output file name
        outfname = f"loop_splitting_times_heatmap_{srcdir}_{experiment}_{access_variant}_{nthreads}threads"
        if variant_dir_suffix != "":
            outfname += variant_dir_suffix
        if args.png:
            outfname += ".png"
        else:
            outfname += ".pdf"

        plt.savefig(outfname, dpi=mydpi)
        print(f"Saved {outfname}")
        plt.close()
