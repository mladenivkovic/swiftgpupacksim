#!/usr/bin/env python3

import os
import matplotlib
from matplotlib import pyplot as plt
import argparse
import numpy as np

matplotlib.use("Agg")

from resultdata import ResultData
from plotting_utils import get_filelist, mydpi, mymplparams, markers, linestyles

matplotlib.rcParams.update(mymplparams)


parser = argparse.ArgumentParser(
    description="""
Plot results of a 'node', i.e. a single top-level directory containing a multitude
of subdirectories, all of which contain experiment outputs for different particle
memory layout variants.

Only takes ouputs (subdirectories) where there is a match between
DIRNAME and DIRNAME_noflush.

This script assumes that all subdirectories in the given `node_root_dir` will
contain result data with filenames 'results_*.csv' It will
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


plotkwargs = {
    "marker": "o",
    "lw": 2,
    "alpha": 0.8,
    "markersize": 5,
}


if __name__ == "__main__":

    args = parser.parse_args()
    node_root_dir = args.node_root_dir
    verbose = args.verbose

    if not os.path.exists(node_root_dir):
        raise FileNotFoundError(f"directory {node_root_dir} not found.")

    ls = os.listdir(node_root_dir)
    dirlist = []

    for f in ls:
        fulldir = os.path.join(node_root_dir, f)

        if fulldir.endswith("_noflush") and os.path.isdir(fulldir):
            flush = fulldir[: -len("_noflush")]
            if os.path.exists(flush):
                # only add dirs to list if there is a flush - noflush pair
                dirlist.append(fulldir)
                dirlist.append(flush)

    fig = plt.figure(figsize=(15, 10))
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

    # loop over all experiments
    colind = 0
    for i in range(len(dirlist)):

        srcdir = dirlist[i]

        color = f"C{colind}"
        ls = "-"
        if not srcdir.endswith("_noflush"):
            ls = ":"
            colind += 1

        # read in data
        filelist = get_filelist(srcdir)
        layouts = [
            os.path.basename(f)[len("results_") : -len(".csv")] for f in filelist
        ]
        layouts.sort()

        result_data = []
        for layout in layouts:

            fname = os.path.join(srcdir, f"results_{layout}.csv")
            res = ResultData(fname, verbose=False)
            result_data.append(res)

        # Unpack result data by packing operation type
        dens_pack = np.array([res.data_dict["pack/density"] for res in result_data])
        dens_unpack = np.array([res.data_dict["unpack/density"] for res in result_data])
        grad_pack = np.array([res.data_dict["pack/gradient"] for res in result_data])
        grad_unpack = np.array(
            [res.data_dict["unpack/gradient"] for res in result_data]
        )
        forc_pack = np.array([res.data_dict["pack/force"] for res in result_data])
        forc_unpack = np.array([res.data_dict["unpack/force"] for res in result_data])

        label = os.path.basename(srcdir)

        ax1.plot(layouts, dens_pack, c=color, ls=ls, label=label, **plotkwargs)
        ax2.plot(layouts, grad_pack, c=color, ls=ls, label=label, **plotkwargs)
        ax3.plot(layouts, forc_pack, c=color, ls=ls, label=label, **plotkwargs)
        ax4.plot(layouts, dens_unpack, c=color, ls=ls, label=label, **plotkwargs)
        ax5.plot(layouts, grad_unpack, c=color, ls=ls, label=label, **plotkwargs)
        ax6.plot(layouts, forc_unpack, c=color, ls=ls, label=label, **plotkwargs)

    # all axes
    for ax in fig.axes:
        #  ax.set_xlabel("particle data layouts")
        ax.tick_params("x", rotation=45)
        ax.grid()

    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 2
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=2.5,
        markerscale=0.5,
    )
    fig.tight_layout(w_pad=0, rect=(0.01, 0.12, 0.99, 0.99))

    # construct output file name
    fullpath = os.path.abspath(node_root_dir)
    node_dir = os.path.basename(fullpath)
    outfname = f"compare_flushing_{node_dir}.png"
    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
