#!/usr/bin/env python3

"""
Stuff related to templates.
"""

import jinja2
import os
from utils import verify_dir_exists, get_git_hash

_root_dir = os.path.dirname(__file__)
_default_template_dir = os.path.join(_root_dir, "templates")


def init_jinja_env(template_dir: str = _default_template_dir) -> jinja2.Environment:
    """
    Initialise the jinja2 environment.
    """

    verify_dir_exists(template_dir)
    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(template_dir), keep_trailing_newline=True
    )

    return env


def jinja_generate_hydro_part_h(
    part_structs_d: dict,
    template_dir: str = _default_template_dir,
    swift_header: bool = True,
    testing: bool = False,
    verbose: bool = False,
) -> str:
    """
    Generates the full hydro_part.h header.

    Parameters
    ----------

    part_structs_d: dict
        dict containing processed data ready to be inserted into the template.
        The dict's keys shall be names of particle structs to be generated.
        Each key shall return a dict with two keys:
            - "API": containing a list of strings containing C-code for the
              particle field API
            - "DECLARATIONS": containing a list of strings containing C-code
              for the particle field declarations

    template_dir: str
        the directory to search for templates

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    testing: bool
        if True, use minimalistic header template for unit tests to generate
        header file

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env(template_dir)
    templ_fname = "hydro_part.h.jinja.template"
    if testing:
        templ_fname = "test_hydro_part.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join(template_dir, templ_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_COUNT"] = len(list(part_structs_d.keys()))
    d["STRUCT_CONTENTS"] = part_structs_d
    d["TEMPLATE_FILENAME"] = templ_full_fname
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()

    header_template = templ.render(d)

    return header_template


def jinja_generate_parts_h(
    part_structs_d: dict,
    template_dir: str = _default_template_dir,
    swift_header: bool = True,
    id_checks: bool=True,
    verbose: bool = False,
) -> str:
    """
    Generates the full parts.h header.

    Parameters
    ----------

    part_structs_d: dict
        dict containing processed data ready to be inserted into the template.
        The dict's keys shall be names of particle data structs to be generated.
        Each key shall return a dict with two keys:
            - "DOC": Struct documentation, if available
            - "HAS_DOC": bool saying whether struct documentation is available

    template_dir: str
        the directory to search for templates

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    id_checks: bool
        If True, add initialisation of particle accessor_id variables

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env(template_dir)
    templ_fname = "parts.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join(template_dir, templ_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_CONTENTS"] = part_structs_d
    d["TEMPLATE_FILENAME"] = templ_full_fname
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()
    d["DEBUG_ID_CHECKS"] = id_checks

    header_template = templ.render(d)

    return header_template


def jinja_generate_hydro_part_arrays_struct_h(
    part_structs_d: dict,
    template_dir: str = _default_template_dir,
    swift_header: bool = True,
    verbose: bool = False,
) -> str:
    """
    Generates the full hydro_part_arrays_struct.h header.

    Parameters
    ----------

    part_structs_d: dict
        dict containing processed data ready to be inserted into the template.
        The dict's keys shall be names of particle data structs to be generated.
        Each key shall return a dict with two keys:
            - "DOC": Struct documentation, if available
            - "HAS_DOC": bool saying whether struct documentation is available

    template_dir: str
        the directory to search for templates

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env(template_dir)
    templ_fname = "hydro_part_arrays_struct.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join(template_dir, templ_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_CONTENTS"] = part_structs_d
    d["TEMPLATE_FILENAME"] = templ_full_fname
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()

    header_template = templ.render(d)

    return header_template
