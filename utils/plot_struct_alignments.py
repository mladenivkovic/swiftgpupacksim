#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse

matplotlib.use("Agg")

from plotting_utils import get_filelist
from resultdata import ResultData


parser = argparse.ArgumentParser(
    description="""
Plot results of different struct alignments for different particle memory
layouts. This script assumes that all subdirectories in the given `srcdir`
will contain result data with filenames 'results_*.csv' It will
read them all in and plot them.

Sub-directories are expected to have the following file name format:
`EXPERIMENTNAME`_`NRTHREADS`threads_`ALIGN`[_noflush]

The *_noflush variants should contain outputs where no cache flushes between
packing operations were used.

Note that the sub-directory `EXPERIMENTNAME`, `NRTHREADS`, and `ALIGN` are
hard-coded in this script. Modify them manually if you need to.
"""
)
parser.add_argument("srcdir")
parser.add_argument(
    "-v", "--verbose", dest="verbose", action="store_true", help="increase verbosity"
)
parser.add_argument(
    "-f", "--flush", dest="use_cache_flush", action="store_true", help="Use output directories where caches were flushed")
parser.add_argument(
    "-l", "--local", dest="local", action="store_true", help="plot outputs for small local test-runs"
)

args = parser.parse_args()
srcdir = args.srcdir
verbose = args.verbose
use_cache_flush = args.use_cache_flush
local = args.local

EXPERIMENT_NAMES = ["EAGLE_12", "GRESHO256"]
NRTHREADS = [16, 32]
STRUCT_ALIGNS = [1, 2, 4, 8, 16, 32, 64]

if local:
    EXPERIMENT_NAMES = ["IntelXeonGold5218_Gresho64"]
    NRTHREADS = [4]


multiple_experiments = False
if len(EXPERIMENT_NAMES) > 1:
    multiple_experiments = True

multiple_threads = False
if len(NRTHREADS) > 1:
    multiple_threads = True



markers = ["o", "v", "s", "p", "P", "*"]
linestyles = ["-", "--", ":"]

if __name__ == "__main__":


    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    # get available layouts
    layouts=[]
    firstdir = EXPERIMENT_NAMES[0] + "_" + str(NRTHREADS[0]) + "threads_struct_align" + str(STRUCT_ALIGNS[0])
    fullfirstdir = os.path.join(srcdir, firstdir)
    ls = os.listdir(fullfirstdir)
    for f in ls:
        if f.startswith("results_") and f.endswith(".csv"):
            layout = f[len("results_"):-len(".csv")]
            layouts.append(layout)


    fig = plt.figure(figsize=(12,6))
    ax1 = fig.add_subplot(2,3,1)
    ax1.set_title("Density/Pack")
    ax2 = fig.add_subplot(2,3,2)
    ax2.set_title("Gradient/Pack")
    ax3 = fig.add_subplot(2,3,3)
    ax3.set_title("Force/Pack")
    ax4 = fig.add_subplot(2,3,4)
    ax4.set_title("Density/Unpack")
    ax5 = fig.add_subplot(2,3,5)
    ax5.set_title("Gradient/Unpack")
    ax6 = fig.add_subplot(2,3,6)
    ax6.set_title("Force/Unpack")


    maxtime = -1.
    mintime = 1e32

    for l, layout in enumerate(layouts):

        for i, name in enumerate(EXPERIMENT_NAMES):
            for j, nthreads in enumerate(NRTHREADS):

                color="C"+str(l)
                marker = markers[i]
                ls = linestyles[j]

                result_data = []

                for k, align in enumerate(STRUCT_ALIGNS):

                    dirname = name + "_" + str(nthreads) + "threads_struct_align" + str(align)
                    fulldirname = os.path.join(srcdir, dirname)
                    if not use_cache_flush:
                        fulldirname += "_noflush"
                    if not os.path.exists(fulldirname):
                        raise FileNotFoundError(f"Experiment output directory {fulldirname} not found.")

                    fname = "results_"+layout+".csv"
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

                label = f"{layout}"
                if multiple_experiments:
                    label+=f"{name}"
                if multiple_threads:
                    label+=f"{nthreads} threads"

                plotkwargs = {
                        "label": label,
                        "c":color,
                        "marker":marker,
                        "ls":ls,
                        "lw":3,
                        "alpha": 0.7,
                        }
                ax1.plot(STRUCT_ALIGNS, dens_pack, **plotkwargs)
                ax2.plot(STRUCT_ALIGNS, grad_pack, **plotkwargs)
                ax3.plot(STRUCT_ALIGNS, forc_pack, **plotkwargs)
                ax4.plot(STRUCT_ALIGNS, dens_unpack, **plotkwargs)
                ax5.plot(STRUCT_ALIGNS, grad_unpack, **plotkwargs)
                ax6.plot(STRUCT_ALIGNS, forc_unpack, **plotkwargs)


    if (mintime < 200. ):
        mintime = 0.

    for ax in fig.axes:
        ax.set_xlabel("struct_align")
        ax.set_ylabel("Timing [ms]")
        ax.grid()
        ax.legend()
        ax.set_ylim(0.9*mintime, 1.1*maxtime)

    fig.tight_layout()

    # construct output file name
    outfname = f"results_{srcdir}"
    if not use_cache_flush:
        outfname += "_noflush"
    outfname += ".png"
    plt.savefig(outfname, dpi=300)
    print(f"Saved {outfname}")



