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
Identify best runtime among loop splitting variants in a heatmap for varying
access methods and memory layouts.

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


args = parser.parse_args()
srcdir = args.srcdir
nthreads = args.nthreads
if isinstance(nthreads, list):
    nthreads = nthreads[0]
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


def plot_heatmap(ax, heatmap):

    im = ax.imshow(heatmap, cmap="viridis_r")
    ax.set_xticks(range(len(PART_ACCESS)), labels=PART_ACCESS_LABELS, rotation=30, ha="right", rotation_mode="anchor")
    ax.set_yticks(range(len(layouts)), labels=layouts,)

    maxval = heatmap.max()
    mean = heatmap.mean()

    # add text annotations
    for l in range(len(layouts)):
        for s in range(len(PART_ACCESS)):
            colour = "black"
            if heatmap[l,s] > 0.5 * (maxval + mean):
                colour = "white"
            text = ax.text(s, l, f"{heatmap[l,s]:.2f}", ha="center", va="center", color=colour)

    return im





if __name__ == "__main__":

    if not os.path.exists(srcdir):
        raise FileNotFoundError(f"directory {srcdir} not found.")

    fig = plt.figure(figsize=(14, 9))
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)
    axes = [ax1, ax2]


    for e, experiment in enumerate(EXPERIMENTS):

        ax = axes[e]
        ax.set_title(experiment)
        print("EXPERIMENT:", experiment)
        print("-------------------------------")
        print()

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
        ref_total_time = res.total_time

        heatmap = np.zeros((len(layouts), len(PART_ACCESS)))

        for a, access in enumerate(PART_ACCESS):

            for l, layout in enumerate(layouts):

                result_data = []
                for s, split in enumerate(LOOP_SPLITS):

                    fname = get_result_fname(
                        srcdir,
                        experiment,
                        nthreads,
                        access,
                        split,
                        variant_dir_suffix,
                        layout,
                    )
                    res = ResultData(fname, verbose=False)
                    result_data.append(res)

                # Unpack result data by packing operation type
                dens_pack = np.array(
                    [r.data_dict["pack/density"] for r in result_data]
                )
                dens_unpack = np.array(
                    [r.data_dict["unpack/density"] for r in result_data]
                )
                grad_pack = np.array(
                    [r.data_dict["pack/gradient"] for r in result_data]
                )
                grad_unpack = np.array(
                    [r.data_dict["unpack/gradient"] for r in result_data]
                )
                forc_pack = np.array(
                    [r.data_dict["pack/force"] for r in result_data]
                )
                forc_unpack = np.array(
                    [r.data_dict["unpack/force"] for r in result_data]
                    )

                dp_best = dens_pack.argmin()
                du_best = dens_unpack.argmin()
                gp_best = grad_pack.argmin()
                gu_best = grad_unpack.argmin()
                fp_best = forc_pack.argmin()
                fu_best = forc_unpack.argmin()

                print("")
                print("-- ", access, "-", layout)
                print("---- dens pack  :", LOOP_SPLITS[dp_best])
                print("---- dens unpack:", LOOP_SPLITS[du_best])
                print("---- grad pack  :", LOOP_SPLITS[gp_best])
                print("---- grad unpack:", LOOP_SPLITS[gu_best])
                print("---- forc pack  :", LOOP_SPLITS[fp_best])
                print("---- forc unpack:", LOOP_SPLITS[fu_best])
                print("")

                heatmap[l, a] = (dens_pack[dp_best] + dens_unpack[du_best] + grad_pack[gp_best] + grad_unpack[gu_best] + forc_pack[fp_best] + forc_unpack[fu_best])
                heatmap[l, a] /= ref_total_time



        im = plot_heatmap(ax, heatmap)

    fig.tight_layout(w_pad=0, rect=(0.01, 0.01, 0.99, 0.99))

    # construct output file name
    outfname = f"loop_splitting_best_times_heatmap_{srcdir}_{nthreads}threads"
    if variant_dir_suffix != "":
        outfname += variant_dir_suffix
    if args.png:
        outfname += ".png"
    else:
        outfname += ".pdf"

    plt.savefig(outfname, dpi=mydpi)
    print(f"Saved {outfname}")
    plt.close()
