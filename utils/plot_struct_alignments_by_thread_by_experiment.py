#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse

matplotlib.use("Agg")

from plotting_utils import get_filelist
from resultdata import ResultData
import numpy as np


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
layouts, making a single output image per experiment and per threadcount. Which
experiments to extract (as given by subdirectory name base) is hardcoded in
this script.

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
parser.add_argument(
    "-p",
    "--png",
    dest="png",
    action="store_true",
    help="make a .png plot instead of .pdf",
)
parser.add_argument(
    "-r",
    "--relative",
    dest="relative",
    action="store_true",
    help="Plot results relative to best case scenario",
)
#  parser.add_argument(
#      "-g",
#      "--grace",
#      dest="grace",
#      action="store_true",
#      help="Use default thread numbers for Grace chip (36, 72)",
#  )
#  parser.add_argument(
#      "-t",
#      "--threads",
#      dest="nthreads",
#      nargs=1,
#      help="Use results of this many threads",
#  )


args = parser.parse_args()
srcdir = args.srcdir
verbose = args.verbose
#  use_cache_flush = args.use_cache_flush
local = args.local

EXPERIMENT_NAMES = ["EAGLE12", "Gresho256"]
#  NRTHREADS=args.nthreads # returns a list.
#  NRTHREADS = [16, 32]
#  if args.grace:
NRTHREADS = ["36", "72"]
STRUCT_ALIGNS = ["1", "2", "4", "8", "16", "32", "64"]

LAYOUT_COMPARE="aos"
ALIGN_COMPARE="32"



datadict_keys = [
    "pack/density",
    "pack/gradient",
    "pack/force",
    "unpack/density",
    "unpack/gradient",
    "unpack/force",
        ]


mintimes_dict = {}
for key in datadict_keys:
    mintimes_dict[key] = 1e32



best_times = {
        "Gresho256": {
            "36": mintimes_dict.copy(),
            "72": mintimes_dict.copy(),
            },
        "EAGLE12": {
            "36": mintimes_dict.copy(),
            "72": mintimes_dict.copy(),
            }
        }


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



    # first, get the times to compare to for threads and for experiments separately
    for nthreads in NRTHREADS:
        for i, name in enumerate(EXPERIMENT_NAMES):

            layout = LAYOUT_COMPARE
            for suffix in dir_suffixes:
                if "_novector" not in suffix:
                    continue

                #  for k, align in enumerate(STRUCT_ALIGNS):
                align = ALIGN_COMPARE

                # read in the results
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

                #  if layout == "upstream" and "_novector" in dirname:
                for key in datadict_keys:
                    if best_times[name][nthreads][key] > res.data_dict[key]:
                        best_times[name][nthreads][key] = res.data_dict[key]
                        print(name, nthreads, key, res.data_dict[key], align)


    # Now make the plots, separately for threads and for experiments
    for nthreads in NRTHREADS:
        for name in EXPERIMENT_NAMES:

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

            # used for constistent y axis limits across all plots.
            maxtime = -1.0
            mintime = 1e32

            index = 0
            for l, layout in enumerate(layouts):

                color = "C" + str(index)
                marker = markers[0]
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

                    dens_pack = np.array([r.data_dict["pack/density"] for r in result_data])
                    dens_unpack = np.array([r.data_dict["unpack/density"] for r in result_data])
                    grad_pack = np.array([r.data_dict["pack/gradient"] for r in result_data])
                    grad_unpack = np.array([r.data_dict["unpack/gradient"] for r in result_data])
                    forc_pack = np.array([r.data_dict["pack/force"] for r in result_data])
                    forc_unpack = np.array([r.data_dict["unpack/force"] for r in result_data])

                    if args.relative:
                        dens_pack /= best_times[name][nthreads]["pack/density"]
                        dens_unpack /= best_times[name][nthreads]["unpack/density"]
                        grad_pack /= best_times[name][nthreads]["pack/gradient"]
                        grad_unpack /= best_times[name][nthreads]["unpack/gradient"]
                        forc_pack /= best_times[name][nthreads]["pack/force"]
                        forc_unpack /= best_times[name][nthreads]["unpack/force"]

                    for arr in [dens_pack, dens_unpack , grad_pack , grad_unpack , forc_pack , forc_unpack]:
                        if mintime > arr.min():
                            mintime = arr.min()
                        if maxtime < arr.max():
                            maxtime = arr.max()

                    label = f" {layout.upper()}"
                    if multiple_threads:
                        label += f" {nthreads} threads"
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

            if mintime < 200.0 and not args.relative:
                mintime = 0.0

            # all axes
            for ax in fig.axes:
                ax.set_xlabel("struct_align")
                ax.grid()
                ax.set_ylim(0.9 * mintime, 1.1 * maxtime)
                if args.relative:
                    ax.set_yscale("log")
                #  ax.legend()

            # leftmost axes
            for ax in [ax1, ax4]:
                if args.relative:
                    ax.set_ylabel(f"Timing relative to {LAYOUT_COMPARE.upper()}-{ALIGN_COMPARE}")
                else:
                    ax.set_ylabel("Timing [ms]")

            # the others
            for ax in [ax2, ax3, ax5, ax6]:
                ax.set_yticklabels([])


            hand, lab = ax1.get_legend_handles_labels()
            ncols=int(len(layouts)*0.5 + 0.5)
            #  ncols = 5
            fig.legend(handles=hand, labels=lab, loc="lower center", ncols=ncols, handlelength=2.5, markerscale=0.5)
            fig.tight_layout(w_pad=0, rect=(0.01, 0.2, 0.99, 0.99))

            # construct output file name
            outfname = f"struct_alignment_exp_{name}_{srcdir}_{nthreads}threads"
            if args.png:
                outfname += ".png"
            else:
                outfname += ".pdf"

            plt.savefig(outfname, dpi=300)
            print(f"Saved {outfname}")


    print(best_times)

