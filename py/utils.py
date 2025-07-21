#!/usr/bin/env python3

import os
from typing import Union


def verify_file_exists(fname: str, message: str = ""):
    """
    Check if file exists, and if not, raise an error.
    Optionally, pass an additional message to be displayed upon error.
    """

    if not os.path.exists(fname):
        raise FileNotFoundError(f"File {fname} doesn't exist. {message}")
    return


def verify_dir_exists(dname: str, message: str = ""):
    """
    Check if file exists, and if not, raise an error.
    Optionally, pass an additional message to be displayed upon error.
    """

    if not os.path.isdir(dname):
        raise FileNotFoundError(f"Directory {dname} doesn't exist. {message}")
    return


def check_output_directory(outdir: Union[str, list], verbose: bool = True):
    """
    Extract the output directory from the parsed args.
    If it doesn't exist, make it.
    """

    if isinstance(outdir, list):
        if len(outdir) > 1:
            raise ValueError("outdir cmdline arg should only contain 1 element?", outdir)
        outdir = outdir[0]
    elif isinstance(outdir, str):
        # we're good
        pass
    else:
        raise ValueError("Unknown type for output_dir cmdline arg:", type(outdir))

    if not os.path.isdir(outdir):
        if verbose:
            print(f"output directory '{outdir}' doesn't exist, creating it.")
        os.makedirs(outdir)

    return outdir


