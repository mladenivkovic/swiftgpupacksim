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
    mymplparams,
    mydpi,
    markers,
    linestyles,
)

matplotlib.rcParams.update(mymplparams)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
Compare RELATIVE DIFFERENCE between packed vs not-packed results for different
particle memory layouts for and differnet loop splitting variants for a given
experiment, and a given particle accessor variant.

By default, this plots the outputs using:
- cache flushes between each copy operation
- no manual vectorization
- 72 threads

See optional flags to modify which results are plotted.

This script assumes that all subdirectories in the given `srcdir` will contain
result data with filenames 'results_*.csv' It will read them all in and plot
them.

Sub-directories are expected to have the following file name format:
<EXPERIMENTNAME>_<NRTHREADS>threads_<PART_ACCESS>_<LOOP_SPLIT>[_packed][_vector][_noflush]

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
    "--png",
    dest="png",
    action="store_true",
    help="make a .png plot instead of .pdf",
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


args = parser.parse_args()
srcdir = args.srcdir
nthreads = args.nthreads
if isinstance(nthreads, list):
    nthreads=nthreads[0]
local = args.local_legion or args.local_hp

if args.equal_axis_limits:
    raise NotImplementedError()
if args.local_hp:
    nthreads = 4
    EXPERIMENTS = ["IntelXeonGold5218_Gresho64"]
if args.local_legion:
    nthreads = 6
    EXPERIMENTS = ["IntelCoffeeLake_Gresho128"]


variant_dir_suffix_nopack, variant_label_suffix_nopack = get_variant_labels(
    args.use_noflush, args.use_vector, False
)
variant_dir_suffix_pack, variant_label_suffix_pack = get_variant_labels(
    args.use_noflush, args.use_vector, True
)

variants = [variant_dir_suffix_nopack, variant_dir_suffix_pack]
variants_labels = [variant_label_suffix_nopack, variant_label_suffix_pack]

plotkwargs = {
    "marker": "o",
    "lw": 2,
    "alpha": 0.6,
    "markersize": 5,
}

if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    # get available layouts
    layouts = []
    firstdir = get_result_dir(
        srcdir, EXPERIMENTS[0], nthreads, PART_ACCESS[0], LOOP_SPLITS[0], other_variant=variants[0]
    )
    ls = os.listdir(firstdir)
    for f in ls:
        if f.startswith("results_") and f.endswith(".csv"):
            layout = f[len("results_") : -len(".csv")]
            layouts.append(layout)
    layouts.sort()

    aos_ind = -1
    for i in range(len(layouts)):
        if layouts[i] == "aos":
            aos_ind = i
            break
    if aos_ind == -1:
        raise ValueError(
            "Something went wrong determining index of AoS in array,", layouts
        )


    fig = plt.figure(figsize=(16, 9))
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

    for e, experiment in enumerate(EXPERIMENTS):

        ls = linestyles[e]
        colorind = 0

        for a, access in enumerate(PART_ACCESS):
            # first, get normalisation: Compare to access="part-struct", loop-split = "none"
            #  dirname = ( experiment + "_" + str(nthreads) + "threads_part-struct_none" + variant_dir_suffix)
            #  fulldirname = os.path.join(srcdir, dirname)
            #  if not os.path.exists(fulldirname):
            #      raise FileNotFoundError(
            #          f"Experiment output directory {fulldirname} not found."
            #      )
            #
            #  fname = "results_aos.csv"
            #  fullfname = os.path.join(fulldirname, fname)
            #  res = ResultData(fullfname, verbose=False)
            #  normalisation = res.data_dict

            for s, split in enumerate(LOOP_SPLITS):
                color = "C" + str(colorind)
                colorind += 1

                # GET NOPACK DATA
                result_data_nopack = []

                for l, layout in enumerate(layouts):
                    fname = get_result_fname(
                        srcdir, experiment, nthreads, access, split, variant_dir_suffix_nopack, layout
                    )
                    res = ResultData(fname, verbose=False)
                    result_data_nopack.append(res)

                    maxtime = max(maxtime, res.timings.max())
                    mintime = min(mintime, res.timings.min())

                # Unpack result data by packing operation type
                dens_pack_nopack = np.array(
                    [res.data_dict["pack/density"] for res in result_data_nopack]
                )
                dens_unpack_nopack = np.array(
                    [res.data_dict["unpack/density"] for res in result_data_nopack]
                )
                grad_pack_nopack = np.array(
                    [res.data_dict["pack/gradient"] for res in result_data_nopack]
                )
                grad_unpack_nopack = np.array(
                    [res.data_dict["unpack/gradient"] for res in result_data_nopack]
                )
                forc_pack_nopack = np.array(
                    [res.data_dict["pack/force"] for res in result_data_nopack]
                )
                forc_unpack_nopack = np.array(
                    [res.data_dict["unpack/force"] for res in result_data_nopack]
                )

                # GET PACK DATA
                result_data_pack = []

                for l, layout in enumerate(layouts):
                    fname = get_result_fname(
                        srcdir, experiment, nthreads, access, split, variant_dir_suffix_pack, layout
                    )
                    res = ResultData(fname, verbose=False)
                    result_data_pack.append(res)

                    maxtime = max(maxtime, res.timings.max())
                    mintime = min(mintime, res.timings.min())

                # Unpack result data by packing operation type
                dens_pack_pack = np.array(
                    [res.data_dict["pack/density"] for res in result_data_pack]
                )
                dens_unpack_pack = np.array(
                    [res.data_dict["unpack/density"] for res in result_data_pack]
                )
                grad_pack_pack = np.array(
                    [res.data_dict["pack/gradient"] for res in result_data_pack]
                )
                grad_unpack_pack = np.array(
                    [res.data_dict["unpack/gradient"] for res in result_data_pack]
                )
                forc_pack_pack = np.array(
                    [res.data_dict["pack/force"] for res in result_data_pack]
                )
                forc_unpack_pack = np.array(
                    [res.data_dict["unpack/force"] for res in result_data_pack]
                )


                label = (
                        experiment + " " +
                    PART_ACCESS_LABELS[a]
                    + " "
                    + LOOP_SPLIT_LABELS[s]
                )

                ax1.plot(
                    layouts, dens_pack_pack/dens_pack_nopack, c=color, ls=ls, label=label, **plotkwargs
                )
                ax2.plot(
                    layouts, grad_pack_pack/grad_pack_nopack, c=color, ls=ls, label=label, **plotkwargs
                )
                ax3.plot(
                    layouts, forc_pack_pack/forc_pack_nopack, c=color, ls=ls, label=label, **plotkwargs
                )
                ax4.plot(
                    layouts, dens_unpack_pack/dens_unpack_nopack, c=color, ls=ls, label=label, **plotkwargs
                )
                ax5.plot(
                    layouts, grad_unpack_pack/grad_unpack_nopack, c=color, ls=ls, label=label, **plotkwargs
                )
                ax6.plot(
                    layouts, forc_unpack_pack/forc_unpack_nopack, c=color, ls=ls, label=label, **plotkwargs
                )

    #  if mintime < 200.0:
    #      mintime = 0.0

    # all axes
    for ax in fig.axes:
        ax.set_xlabel("particle data layouts")
        ax.tick_params("x", rotation=45)
        ax.grid()
        #  ax.legend()
        if args.equal_axis_limits:
            ax.set_ylim(0.9 * mintime, 1.1 * maxtime)

    # leftmost axes
    for ax in [ax1, ax4]:
        ax.set_ylabel(
            r"$t / t_{\mathrm{pack}} / t_{\mathrm{unpack}}$"
        )

    # top row axes
    for ax in [ax1, ax2, ax3]:
        ax.set_xticklabels([])
        ax.set_xlabel(None)

    # the others
    for ax in [ax2, ax3, ax5, ax6]:
        if args.equal_axis_limits:
            ax.set_yticklabels([])

    hand, lab = ax1.get_legend_handles_labels()
    #  ncols=int(len(layouts)*0.5 + 0.5)
    ncols = 4
    fig.legend(
        handles=hand,
        labels=lab,
        loc="lower center",
        ncols=ncols,
        handlelength=2.5,
        markerscale=0.5,
    )
    fig.tight_layout(w_pad=0, rect=(0.01, 0.15, 0.99, 0.99))

    # construct output file name
    outfname = f"loop_splitting_compare_packed_{srcdir}_{access}"
    if variant_dir_suffix_nopack != "":
        outfname += variant_dir_suffix_nopack
    outfname += "_relative"
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
    plt.close()
