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

The *_noflush variants will be overplotted with the results in subdirs with the
same name without that suffix.

Note that the sub-directory `EXPERIMENTNAME`, `NRTHREADS`, and `ALIGN` are
hard-coded in this script. Modify them manually if you need to.
"""
)
parser.add_argument("srcdir")
parser.add_argument(
    "-v", "--verbose", dest="verbose", action="store_true", help="increase verbosity"
)
parser.add_argument(
    "-i", "--ignore-cache-flush", dest="ignore_cache_flush", action="store_true", help="ignore experiment outputs with cache flushes."
)


EXPERIMENT_NAMES = ["EAGLE_12", "GRESHO256"]
NRTHREADS = [16, 32]
STRUCT_ALIGNS = [1, 2, 4, 8, 16, 32, 64]

markers = ["o", "v", "s", "p", "P", "*"]

if __name__ == "__main__":

    args = parser.parse_args()
    srcdir = args.srcdir
    verbose = args.verbose
    ignore_cache_flush = args.ignore_cache_flush

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")


    fig = plt.figure(figsize=(10,10))
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

    # loop over all experiments
    for i,name in enumerate(EXPERIMENT_NAMES):
        for j,nthreads in enumerate(NRTHREADS):
            for k, align in enumerate(STRUCT_ALIGNS):
                dirname = name + "_" + str(nthreads) + "_struct_align" + str(align)
                fulldirname = os.path.join(srcdir, dirname)
                if not os.path.exists(fulldirname):
                    raise FileNotFoundError(f"Experiment output directory {fulldirname} not found.")
                if not ignore_cache_flush:
                    noflushfulldirname = fulldirname + "_noflush"
                    if not os.path.exists(noflushfulldirname):
                        raise FileNotFoundError(f"Experiment output directory {noflushfulldirname} not found.")





    for i in range(len(flushlist)):

        flushdir = flushlist[i]
        noflushdir = noflushlist[i]
        ax = fig.axes[i]

        title = os.path.basename(flushdir)

        # do both flush + no flush
        for j,srcdir in enumerate([flushdir, noflushdir]):

            #  marker = "o"
            marker = markers[j]
            label_suffix = ""
            linestyle="-"

            if j == 1:
                #  marker = "x"
                label_suffix=" no cache flush"
                linestyle="--"
                if not srcdir.endswith("_noflush"):
                    raise ValueError(f"Expected noflush dir case, got {srcdir}")

            # read in data
            filelist = get_filelist(srcdir)

            resultlist = []
            for f in filelist:
                res = ResultData(f, verbose=verbose)
                resultlist.append(res)

            # find the AOS results
            aos_index = -1
            for i, r in enumerate(resultlist):
                if r.layout == "aos":
                    aos_index = i
                    break

            if aos_index == -1:
                raise ValueError("Didn't find AOS results")

            aos_timings = resultlist[aos_index].timings

            for k,res in enumerate(resultlist):

                color = "C"+str(k)
                marker = markers[k]

                relative_times = res.timings / aos_timings
                ax.plot(res.tasks, relative_times, label=res.layout + label_suffix, marker=marker, linestyle=linestyle, color=color)

        ax.set_title(title)
        ax.tick_params(axis='x', labelrotation=45)
        ax.legend()
        ax.grid()
        plt.tight_layout()

    # construct output file name
    fullpath = os.path.abspath(node_root_dir)
    node_dir = os.path.basename(fullpath)
    outfname = f"results_{node_dir}.png"
    plt.savefig(outfname)
    print(f"Saved {outfname}")



