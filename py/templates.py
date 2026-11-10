#!/usr/bin/env python3

"""
Stuff related to templates.
"""

import jinja2
import os
from utils import verify_dir_exists, get_git_hash, truncate_repo_file_path

_root_dir = os.path.dirname(__file__)
_default_template_dir = os.path.join(_root_dir, "templates")


def init_jinja_env(template_dir: str = "") -> jinja2.Environment:
    """
    Initialise the jinja2 environment.

    Parameters
    ----------

    template_dir: str
        which directory to read templates from.
        If `template_dir` is a relative path, it will be appended to
        `_default_template_dir`. If it is an absolute path, it will be used as
        a full path to the directory instead.

    Returns
    -------

    env: jinja2.Environment
        A set-up jinja2 environment to be used to render templates
    """

    tdir = _default_template_dir
    if template_dir != "":
        if os.path.isabs(template_dir):
            tdir = template_dir
        else:
            tdir = os.path.join(tdir, template_dir)


    verify_dir_exists(tdir)
    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(tdir), keep_trailing_newline=True
    )

    # add 'ord' and 'chr' builtin to environment so I can use it within the
    # templates to make i, j, k indices for multidim arrays
    env.globals.update(ord=ord)
    env.globals.update(chr=chr)
    return env


def jinja_generate_hydro_part_dot_h(
    part_structs_d: dict,
    metadata_d: dict,
    swift_header: bool = True,
    manual_struct_align: bool = False,
    explicit_var_accessors: bool = False,
    global_var_accessors: bool = False,
    part_struct_accessors: bool = False,
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

    metadata_d: dict
        dict containing particle (flavour) metadata

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    manual_struct_align: bool
        if True, generate a different SWIFT_STRUCT_ALIGN_ macro for each struct.

    explicit_var_accessors: bool
        if True, generate getters/setters which use explicitly passed pointer
        as an argument

    global_var_accessors: bool
        if True, also generate getters/setters which use global variable pointer

    part_struct_accessors: bool
        if True, generate getters/setters using a part struct

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

    env = init_jinja_env("files")
    templ_fname = "hydro_part.h.jinja.template"
    if testing:
        templ_fname = "test_hydro_part.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join("templates", "files", templ_fname)
    templ_trunc_name= truncate_repo_file_path(templ_full_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_COUNT"] = len(list(part_structs_d.keys()))
    d["STRUCT_CONTENTS"] = part_structs_d

    d["TEMPLATE_FILENAME"] = templ_trunc_name
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()
    d["MANUAL_STRUCT_ALIGN"] = manual_struct_align
    d["GLOBAL_VAR_ACCESSORS"] = global_var_accessors
    d["EXPLICIT_VAR_ACCESSORS"] = explicit_var_accessors
    d["PART_STRUCT_ACCESSORS"] = part_struct_accessors

    d["AUTHORS"] = metadata_d["authors"]
    d["FLAVOUR_NAME"] = metadata_d["flavour"].upper()
    d["HAS_FILE_DOC"] = metadata_d["has_doc"]
    d["FILE_DOC"] = metadata_d["doc"]
    d["INCLUDES"] = metadata_d["includes"]
    d["HAS_EXTRA_INCLUDES"] = metadata_d["has_extra_includes"]
    d["EXTRA_INCLUDES"] = metadata_d["includes_add"]
    d["INPUT_FILENAME"] = metadata_d["input_file"]

    header_template = templ.render(d)

    return header_template


def jinja_generate_parts_dot_h(
    part_structs_d: dict,
    metadata_d: dict,
    swift_header: bool = True,
    id_checks: bool = True,
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

    metadata_d: dict
        dict containing particle (flavour) metadata

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

    env = init_jinja_env("files")
    templ_fname = "parts.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join("templates", "files", templ_fname)
    templ_trunc_name= truncate_repo_file_path(templ_full_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_CONTENTS"] = part_structs_d
    d["TEMPLATE_FILENAME"] = templ_trunc_name
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()
    d["DEBUG_ID_CHECKS"] = id_checks
    d["INPUT_FILENAME"] = metadata_d["input_file"]

    header_template = templ.render(d)

    return header_template


def jinja_generate_hydro_part_arrays_struct_dot_h(
    part_structs_d: dict,
    metadata_d: dict,
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

    metadata_d: dict
        dict containing particle (flavour) metadata

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env("files")
    templ_fname = "hydro_part_arrays_struct.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join("templates", "files", templ_fname)
    templ_trunc_name= truncate_repo_file_path(templ_full_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_CONTENTS"] = part_structs_d
    d["TEMPLATE_FILENAME"] = templ_trunc_name
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()
    d["INPUT_FILENAME"] = metadata_d["input_file"]

    header_template = templ.render(d)

    return header_template


def jinja_generate_hydro_part_arrays_flush_dot_h(
    part_structs_d: dict,
    metadata_d: dict,
    swift_header: bool = True,
    verbose: bool = False,
) -> str:
    """
    Generates the full hydro_part_arrays_flush.h header.

    Parameters
    ----------

    part_structs_d: dict
        dict containing processed data ready to be inserted into the template.
        The dict's keys shall be names of particle data structs to be generated.
        Each key shall return a dict with two keys:
            - "DOC": Struct documentation, if available
            - "HAS_DOC": bool saying whether struct documentation is available

    metadata_d: dict
        dict containing particle (flavour) metadata

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env("files")
    templ_fname = "hydro_part_arrays_flush.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join("templates", "files", templ_fname)
    templ_trunc_name= truncate_repo_file_path(templ_full_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(part_structs_d.keys())
    d["STRUCT_CONTENTS"] = part_structs_d
    d["TEMPLATE_FILENAME"] = templ_trunc_name
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()
    d["INPUT_FILENAME"] = metadata_d["input_file"]

    header_template = templ.render(d)

    return header_template


def jinja_generate_hydro_space_dot_c(
    part_structs_d: dict,
    metadata_d: dict,
    swift_header: bool = True,
    verbose: bool = False,
) -> str:
    """
    Generates the full hydro_part_arrays_flush.h header.

    Parameters
    ----------

    part_structs_d: dict
        dict containing processed data ready to be inserted into the template.

    metadata_d: dict
        dict containing particle (flavour) metadata

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env("files")
    templ_fname = "hydro_space.c.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join("templates", "files", templ_fname)
    templ_trunc_name= truncate_repo_file_path(templ_full_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = part_structs_d["names"]
    d["TEMPLATE_FILENAME"] = templ_trunc_name
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()

    d["INPUT_FILENAME"] = metadata_d["input_file"]
    d["FLAVOUR_NAME"] = metadata_d["flavour"].upper()

    basefile_template = templ.render(d)

    return basefile_template

def jinja_generate_timestep_limiter_dot_h(
    limiter_structs_d: dict,
    metadata_d: dict,
    swift_header: bool = True,
    manual_struct_align: bool = False,
    explicit_var_accessors: bool = False,
    global_var_accessors: bool = False,
    part_struct_accessors: bool = False,
    testing: bool = False,
    verbose: bool = False,
) -> str:
    """
    Generates the full hydro_part.h header.

    Parameters
    ----------

    TODO: FIX DOCU

    limiter_structs_d: dict
        dict containing processed data ready to be inserted into the template.
        The dict's keys shall be names of struct to be generated.
        Each key shall return a dict with two keys:
            - "API": containing a list of strings containing C-code for the
              particle field API
            - "DECLARATIONS": containing a list of strings containing C-code
              for the particle field declarations

    metadata_d: dict
        dict containing particle (flavour) metadata

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    explicit_var_accessors: bool
        if True, generate getters/setters which use explicitly passed pointer
        as an argument

    global_var_accessors: bool
        if True, also generate getters/setters which use global variable pointer

    part_struct_accessors: bool
        if True, generate getters/setters using a part struct

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_template: str
        the rendered template as a string
    """

    env = init_jinja_env("files")
    templ_fname = "timestep_limiter_struct.h.jinja.template"
    templ = env.get_template(templ_fname)
    templ_full_fname = os.path.join("templates", "files", templ_fname)
    templ_trunc_name= truncate_repo_file_path(templ_full_fname)

    # fill up dict for template rendering
    d = {}
    d["STRUCT_NAMES"] = list(limiter_structs_d.keys())
    d["STRUCT_COUNT"] = len(list(limiter_structs_d.keys()))
    d["STRUCT_CONTENTS"] = limiter_structs_d
    d["TEMPLATE_FILENAME"] = templ_trunc_name
    d["HEADER_FOR_SWIFT"] = swift_header
    d["HEADER_GUARD"] = get_git_hash()
    d["MANUAL_STRUCT_ALIGN"] = manual_struct_align
    d["GLOBAL_VAR_ACCESSORS"] = global_var_accessors
    d["EXPLICIT_VAR_ACCESSORS"] = explicit_var_accessors
    d["PART_STRUCT_ACCESSORS"] = part_struct_accessors

    d["AUTHORS"] = metadata_d["authors"]
    d["FLAVOUR_NAME"] = metadata_d["flavour"].upper()
    d["HAS_FILE_DOC"] = metadata_d["has_doc"]
    d["FILE_DOC"] = metadata_d["doc"]
    d["INCLUDES"] = metadata_d["includes"]
    d["HAS_EXTRA_INCLUDES"] = metadata_d["has_extra_includes"]
    d["EXTRA_INCLUDES"] = metadata_d["includes_add"]
    d["INPUT_FILENAME"] = metadata_d["input_file"]

    header_template = templ.render(d)

    return header_template



