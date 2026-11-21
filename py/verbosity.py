#!/usr/bin/env python3

# Note: In order to be modifyable from main, this needs to be
# stored in a mutable object, not e.g. a boolean literal.
verbose_settings = {"verbose": False, "debug": False}

import pprint


def vprint(*args, **kwargs) -> None:
    """
    Print if verbosity is switched on.
    Use exactly as you would use print().
    """

    if verbose_settings["verbose"] or verbose_settings["debug"]:
        print("[VERB]", *args, **kwargs)
    return


def vpprint(*args, **kwargs) -> None:
    """
    Pretty-print if verbosity is switched on.
    Use exactly as you would use pprint().
    """

    if verbose_settings["verbose"] or verbose_settings["debug"]:
        pprint.pprint(*args, **kwargs)
    return


def dprint(*args, **kwargs) -> None:
    """
    Print if verbosity is switched on.
    Use exactly as you would use print().
    """

    if verbose_settings["debug"]:
        print("[DEBG]", *args, **kwargs)
    return


def dpprint(*args, **kwargs) -> None:
    """
    Pretty-print if verbosity is switched on.
    Use exactly as you would use pprint().
    """

    if verbose_settings["debug"]:
        pprint.pprint(*args, **kwargs)
    return
