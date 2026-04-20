#!/usr/bin/env python3

import os

LOOP_SPLITS = ["none", "by-struct", "by-element", "by-type", "by-struct-and-type"]
LOOP_SPLIT_LABELS = ["no loop split", "loop split by struct", "loop split by element", "loop split by data type", "loop split by struct and data type"]

PART_ACCESS = ["part-struct", "explicit-var", "global-var"]
PART_ACCESS_LABELS = ["part struct access", "explicit var access", "global var access"]



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
