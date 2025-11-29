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
Plot results of a node. This script assumes that all subdirectories in the given
`node_root_dir` will contain result data with filenames 'results_*.csv' It will
read them all in and plot them.
Subdirectories are expected to have the following file name format:
`EXPERIMENTNAME`_`NRTHREADS`threads[_noflush]
The *_noflush variants will be overplotted with the results in subdirs with the
same name without that suffix.
"""
)
parser.add_argument("node_root_dir")
parser.add_argument(
    "-v", "--verbose", dest="verbose", action="store_true", help="increase verbosity"
)



if __name__ == "__main__":

    args = parser.parse_args()
    node_root_dir = args.node_root_dir
    verbose = args.verbose

    if not os.path.exists(node_root_dir):
        raise FileNotFoundError(f"directory {node_root_dir} not found.")

    ls = os.listdir(node_root_dir)
    noflushlist = []
    flushlist = []
    for f in ls:
        if f.endswith("_noflush"):
            noflushlist.append(os.path.join(node_root_dir, f))

            flush = f[:-len("_noflush")]
            fullflush = os.path.join(node_root_dir, flush)
            if not os.path.exists(fullflush):
                raise FileNotFoundError(f"Didn't find corresponing dir {flush}")

            flushlist.append(fullflush)

    if len(flushlist) != 4:
        raise ValueError(f"Found {len(flushlist)} subdirs, expected 4. Adapt script to accommodate that.")

    fig = plt.figure(figsize=(10,10))
    ax1 = fig.add_subplot(2,2,1)
    ax2 = fig.add_subplot(2,2,2)
    ax3 = fig.add_subplot(2,2,3)
    ax4 = fig.add_subplot(2,2,4)


    for i in range(len(flushlist)):

        flushdir = flushlist[i]
        noflushdir = noflushlist[i]
        ax = fig.axes[i]


        title = os.path.basename(flushdir)

        for j,srcdir in enumerate([flushdir, noflushdir]):

            #  color = "C"+str(i)
            marker = "o"
            label_suffix = ""

            if j == 1:
                marker = "x"
                label_suffix=" no cache flush"
                if not srcdir.endswith("_noflush"):
                    raise ValueError(f"Expected noflush dir case, got {srcdir}")

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

            for res in resultlist:

                relative_times = res.timings / aos_timings
                ax.plot(res.tasks, relative_times, label=res.layout + label_suffix, marker=marker)

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



