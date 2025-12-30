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
Plot results of different struct alignments for different particle memory
layouts, making a single output image per experiment. Which experiments to
extract (as given by subdirectory name base) is hardcoded in this script.

This script assumes that all subdirectories in the given `srcdir` will contain
result data with filenames 'results_*.csv' It will read them all in and plot
them.

Sub-directories are expected to have the following file name format:
`EXPERIMENTNAME`_`NRTHREADS`threads_`ALIGN`[_noflush][_novector]

The *_noflush variants should contain outputs where no cache flushes between
packing operations were used. Similarly for *_novector, where code compiled
with --disable-vectorization was used.

Note that the sub-directory `EXPERIMENTNAME`, `NRTHREADS`, and `ALIGN` are
hard-coded in this script. Modify them manually if you need to.
"""
)
parser.add_argument("srcdir")
parser.add_argument(
    "-v", "--verbose", dest="verbose", action="store_true", help="increase verbosity"
)
#  parser.add_argument(
#      "-f",
#      "--flush",
#      dest="use_cache_flush",
#      action="store_true",
#      help="Use output directories where caches were flushed",
#  )
parser.add_argument(
    "-l",
    "--local",
    dest="local",
    action="store_true",
    help="plot outputs for small local test-runs",
)

args = parser.parse_args()
srcdir = args.srcdir
verbose = args.verbose
#  use_cache_flush = args.use_cache_flush
local = args.local

EXPERIMENT_NAMES = ["EAGLE_12", "GRESHO256"]
NRTHREADS = [16, 32]
STRUCT_ALIGNS = ["1", "2", "4", "8", "16", "32", "64"]

if local:
    EXPERIMENT_NAMES = ["IntelXeonGold5218_Gresho64"]
    NRTHREADS = [4]


multiple_threads = False
if len(NRTHREADS) > 1:
    multiple_threads = True


markers = ["o", "v", "s", "p", "P", "*"]
linestyles = ["-", "--", ":", "-."]
dir_suffixes = ["", "_noflush", "_novector", "_noflush_novector"]
suffix_labels = ["", " no flush", " no vector", " no flush, no vector"]


if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    # get available layouts
    layouts = []
    firstdir = (
        EXPERIMENT_NAMES[0]
        + "_"
        + str(NRTHREADS[0])
        + "threads_struct_align"
        + str(STRUCT_ALIGNS[0])
    )
    fullfirstdir = os.path.join(srcdir, firstdir)
    ls = os.listdir(fullfirstdir)
    for f in ls:
        if f.startswith("results_") and f.endswith(".csv"):
            layout = f[len("results_") : -len(".csv")]
            layouts.append(layout)

    # loop over experiments, create image each
    for i, name in enumerate(EXPERIMENT_NAMES):

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

        index = 0
        for l, layout in enumerate(layouts):
            for j, nthreads in enumerate(NRTHREADS):

                color = "C" + str(index)
                marker = markers[i]
                index += 1

                for s, suffix in enumerate(dir_suffixes):

                    ls = linestyles[s]

                    result_data = []

                    for k, align in enumerate(STRUCT_ALIGNS):

                        dirname = (
                            name + "_" + str(nthreads) + "threads_struct_align" + str(align)
                        )
                        dirname += suffix
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
                    dens_unpack = [res.data_dict["unpack/density"] for res in result_data]
                    grad_pack = [res.data_dict["pack/gradient"] for res in result_data]
                    grad_unpack = [res.data_dict["unpack/gradient"] for res in result_data]
                    forc_pack = [res.data_dict["pack/force"] for res in result_data]
                    forc_unpack = [res.data_dict["unpack/force"] for res in result_data]

                    label = f"{layout.upper()}"
                    if multiple_threads:
                        label += f"{nthreads} threads"
                    label += suffix_labels[s]

                    plotkwargs = {
                        "label": label,
                        "c": color,
                        "marker": marker,
                        "ls": ls,
                        "lw": 2,
                        "alpha": 0.8,
                    }
                    ax1.plot(STRUCT_ALIGNS, dens_pack, **plotkwargs)
                    ax2.plot(STRUCT_ALIGNS, grad_pack, **plotkwargs)
                    ax3.plot(STRUCT_ALIGNS, forc_pack, **plotkwargs)
                    ax4.plot(STRUCT_ALIGNS, dens_unpack, **plotkwargs)
                    ax5.plot(STRUCT_ALIGNS, grad_unpack, **plotkwargs)
                    ax6.plot(STRUCT_ALIGNS, forc_unpack, **plotkwargs)

        if mintime < 200.0:
            mintime = 0.0

        # all axes
        for ax in fig.axes:
            ax.set_xlabel("struct_align")
            ax.grid()
            ax.set_ylim(0.9 * mintime, 1.1 * maxtime)
            #  ax.legend()

        # leftmost axes
        for ax in [ax1, ax4]:
            ax.set_ylabel("Timing [ms]")

        # the others
        for ax in [ax2, ax3, ax5, ax6]:
            ax.set_yticklabels([])


        hand, lab = ax1.get_legend_handles_labels()
        #  ncols=int(len(layouts)*0.5 + 0.5)
        ncols = 5
        fig.legend(handles=hand, labels=lab, loc="lower center", ncols=ncols, handlelength=2.5, markerscale=0.5)
        fig.tight_layout(w_pad=0, rect=(0.01, 0.15, 0.99, 0.99))

        # construct output file name
        outfname = f"struct_alignment_exp_{name}_{srcdir}"
        outfname += ".png"
        plt.savefig(outfname, dpi=300)
        print(f"Saved {outfname}")
