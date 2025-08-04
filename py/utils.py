#!/usr/bin/env python3

import os
from typing import Union
import subprocess
import datetime


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
            raise ValueError(
                "outdir cmdline arg should only contain 1 element?", outdir
            )
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


def get_git_hash() -> str:
    """
    Get the git hash of the latest commit.
    If git is unavailable or command fails for any reason, return current time instead.
    """

    git_hash_cmd = "git rev-parse --verify HEAD"
    try:
        githash = subprocess.run(
            git_hash_cmd,
            shell=True,
            check=True,
            capture_output=True,
        )
        gitstdout = githash.stdout
        if isinstance(gitstdout, bytes):
            gitstdout = gitstdout.decode("utf8")
        hashstr = gitstdout.strip()
    except subprocess.CalledProcessError:
        print("Something went wrong when trying to obtain hash of latest git commit.")
        print(f"Command that failed was '{git_hash_cmd}'")
        print("Using current timestamp instead.")
        hashstr = datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S_%f")

    return hashstr

def print_separator(msg: str=""):
    print("----------------------------------------------------------------", msg)
    return

def validate_yml_contents(contents_d: dict) -> None:
    """
    Run through the read-in data from the yml file, passed as the dict
    `contents_d`, and validate that there is no issues:

    (- no top level struct defined twice)
    - no fields with same name defined twice
    """

    # This doesn't really work, since the yaml reader just overwrites them.
    #  struct_names = []
    #  for key in list(contents_d.keys()):
    #
    #      print(key)
    #      if key in struct_names:
    #          raise ValueError(f"particle data struct {key} defined more than once in your input yml file.")
    #      struct_names.append(key)

    field_names = []
    for key in list(contents_d.keys()):

        struct_fields = contents_d[key]
        for field in list(struct_fields.keys()):
            if field in ["doc", "documentation"]:
                # These are allowed to exist multiple times.
                continue
            if field in field_names:
                raise ValueError(f"data field '{field}' defined more than once in your input yml file.")
            field_names.append(field)

    return



