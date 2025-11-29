#!/usr/bin/env python3

import os


def get_filelist(srcdir:str) -> list:
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
