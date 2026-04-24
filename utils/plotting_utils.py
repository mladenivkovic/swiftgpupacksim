#!/usr/bin/env python3

import os

LOOP_SPLITS = ["none", "by-struct", "by-element", "by-type", "by-struct-and-type"]
LOOP_SPLIT_LABELS = ["no loop split", "loop split by struct", "loop split by element", "loop split by data type", "loop split by struct and data type"]

PART_ACCESS = ["part-struct", "explicit-var", "global-var"]
PART_ACCESS_LABELS = ["part struct access", "explicit var access", "global var access"]

EXPERIMENTS = ["Gresho256", "EAGLE12"]
NTHREADS = ["36", "72"]



# Plot parameters
mymplparams = {
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

mydpi = 120

markers = ["o", "v", "s", "p", "P", "*"]
linestyles = ["-", "--", ":", "-."]




def get_filelist(srcdir: str) -> list:
    """
    Get list of results_*.csv files in `srcdir`
    """

    ls = os.listdir(srcdir)
    filelist = []
    for f in ls:
        if f.startswith("results_") and f.endswith(".csv"):
            filelist.append(os.path.join(srcdir, f))

    if len(filelist) == 0:
        raise ValueError(f"Found no results_*.csv files in {srcdir}")

    return filelist


def get_variant_labels(use_noflush, use_vector, use_packed):
    """
    Get suffix and labels for run variants.
    """

    variant_dir_suffix = ""
    variant_label_suffix = ""
    if use_noflush:
        variant_dir_suffix += "_noflush"
        variant_label_suffix += ", no flush"
    if use_vector:
        variant_dir_suffix += "_vector"
        variant_label_suffix += ", vector"
    if use_packed:
        variant_dir_suffix += "_packed"
        variant_label_suffix += ", packed structs"

    return variant_dir_suffix, variant_label_suffix

def get_result_dir(srcdir, experiment_name, nthreads, access_variant, loop_split_variant, other_variant = ""):
    """
    Get the full directory path of an experiment output directory.
    """

    dirname = (
        experiment_name
        + "_"
        + str(nthreads)
        + "threads_"
        + access_variant + "_" + loop_split_variant + other_variant
    )

    fulldirname = os.path.join(srcdir, dirname)
    if not os.path.exists(fulldirname):
        raise FileNotFoundError(
            f"Experiment output directory {fulldirname} not found."
        )

    return fulldirname


def get_result_fname(srcdir, experiment_name, nthreads, access_variant, loop_split_variant, other_variant, layout):
    """
    Construct the experiment output file name.
    """
    fulldirname = get_result_dir(srcdir, experiment_name, nthreads, access_variant, loop_split_variant, other_variant)

    fname = "results_" + layout + ".csv"
    fullfname = os.path.join(fulldirname, fname)
    return fullfname


