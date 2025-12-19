#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse

matplotlib.use("Agg")

from plotting_utils import get_filelist
from resultdata import ResultData


parser = argparse.ArgumentParser(
    description="Plot results of a single dir. This script will look for all results_*csv files in that dir and plot them in a single plot."
)
parser.add_argument("result_dir")
parser.add_argument(
    "-v", "--verbose", dest="verbose", action="store_true", help="increase verbosity"
)


if __name__ == "__main__":

    args = parser.parse_args()
    srcdir = args.result_dir
    verbose = args.verbose

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

    fig = plt.figure(figsize=(6, 4))
    ax = fig.add_subplot(1, 1, 1)

    for res in resultlist:

        relative_times = res.timings / aos_timings
        ax.plot(res.tasks, relative_times, label=res.layout, marker="o")

    ax.tick_params(axis="x", labelrotation=45)
    ax.legend()
    ax.grid()
    plt.tight_layout()

    # construct output file name
    fullpath = os.path.abspath(srcdir)
    measure_dir = os.path.basename(fullpath)
    outfname = f"results_{measure_dir}.png"
    plt.savefig(outfname)
    print(f"Saved {outfname}")
