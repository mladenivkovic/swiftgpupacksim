#!/usr/bin/env python3

import os


def _verify_file_exists(fname: str, message: str = ""):
    """
    Check if file exists, and if not, raise an error.
    """

    if not os.path.exists(fname):
        raise FileNotFoundError(f"File {fname} doesn't exist. {message}")
    return
