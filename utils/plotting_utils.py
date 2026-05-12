#!/usr/bin/env python3

import os

LOOP_SPLITS = ["none", "by-struct", "by-element", "by-type", "by-struct-and-type"]
LOOP_SPLIT_LABELS = [
    "no loop split",
    "loop split by struct",
    "loop split by element",
    "loop split by data type",
    "loop split by struct and data type",
]

PART_ACCESS = ["part-struct", "explicit-var", "global-var"]
PART_ACCESS_LABELS = ["part struct access", "explicit var access", "global var access"]

#  EXPERIMENTS = ["Gresho256", "EAGLE12"]
EXPERIMENTS = ["Gresho256", "EAGLE25"]
NTHREADS = ["36", "72"]

#  NODES = ["dine2", "gn003"]
NODES = ["dine2_5steps", "gn003_5steps"]
NODE_LABELS = {
        "dine2": "dine2",
        "dine2_5steps": "dine2",
        "gn003": "gracehopper",
        "gn003_5steps": "gracehopper",
        }

LAYOUTS_TO_USE = [
        "aos",
        "random-order",
        "pack-gradient",
        "pack-force",
        "pack-shared",
        "pack-gradient-type",
        "pack-force-type",
        "pack-shared-type",
        "soa",
        #  "upstream",
        ]

LAYOUTS_TO_USE_MINIMAL = [
        "aos",
        #  "random-order",
        "pack-gradient",
        "pack-shared",
        "pack-force",
        #  "pack-force-type",
        "soa",
        #  "upstream",
        ]



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
    #  "figure.subplot.wspace": 0.0,
    #  "figure.subplot.hspace": 0.12,
}

mydpi = 120

markers = ["o", "v", "s", "p", "P", "*"]
linestyles = ["-", ":", "--", "-."]


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
    if use_packed:
        variant_dir_suffix += "_packed"
        variant_label_suffix += ", packed structs"
    if use_vector:
        variant_dir_suffix += "_vector"
        variant_label_suffix += ", vector"
    if use_noflush:
        variant_dir_suffix += "_noflush"
        variant_label_suffix += ", no flush"

    return variant_dir_suffix, variant_label_suffix


def get_result_dir(
    srcdir,
    experiment_name,
    nthreads,
    access_variant,
    loop_split_variant,
    other_variant="",
    check_dir_exists=True,
):
    """
    Get the full directory path of an experiment output directory.
    """

    if other_variant != "" and not other_variant.startswith("_"):
        other_variant = "_" + other_variant

    dirname = (
        experiment_name
        + "_"
        + str(nthreads)
        + "threads_"
        + access_variant
        + "_"
        + loop_split_variant
        + other_variant
    )

    fulldirname = os.path.join(srcdir, dirname)
    if check_dir_exists and not os.path.exists(fulldirname):
        raise FileNotFoundError(f"Experiment output directory {fulldirname} not found.")

    return fulldirname


def get_result_fname(
    srcdir,
    experiment_name,
    nthreads,
    access_variant,
    loop_split_variant,
    other_variant,
    layout,
):
    """
    Construct the experiment output file name.
    """
    fulldirname = get_result_dir(
        srcdir,
        experiment_name,
        nthreads,
        access_variant,
        loop_split_variant,
        other_variant,
    )

    fname = "results_" + layout + ".csv"
    fullfname = os.path.join(fulldirname, fname)
    return fullfname
