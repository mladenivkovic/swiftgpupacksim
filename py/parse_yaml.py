#!/usr/bin/env python3

import yaml
import sys
from typing import Union
from templates import init_jinja_env


# List of all permissible field descriptors for a particle field.
# Used for consistency checks.
_allowed_field_descriptors = [
    "type",
    "size",
    "doc",
    "documentation",
    "contents",
    "ifdef",
    "ifdef_return_val",
]

# List of all permissible particle field data types.
# Used for consistency checks.
_allowed_field_data_types = [
    "int",
    "long",
    "long long",
    "float",
    "double",
    "char",
    "integertime_t",
    "timebin_t",
    "struct",
    "union",
]


# Default return values for variables hidden behind macros
_field_data_type_default_return_vals = {
    "int": "INT_MAX",
    "long": "LONG_MAX",
    "long long": "LLONG_MAX",
    "float": "FLT_MAX",
    "double": "DBL_MAX",
    "char": "CHAR_MAX",
    "integertime_t": "LLONG_MAX",
    "timebin_t": "CHAR_MAX",
    "struct": None,
    "union": None,
    "pointer": "NULL",
}


class FieldEntry(object):
    """
    A class to store a particle field entry from the yaml
    input files.
    """

    # jinja environment used to load and generate templates
    jinja_env = init_jinja_env()
    # used to give unions unique identifier in dicts
    union_count = 0

    def __init__(
        self,
        field_name: str,
        field_props: Union[dict, None],
        verbose=False,
        recursion_level=0,
    ):
        """
        Parameters
        ----------

        field_name: str
            name of the particle field to be extracted.

        field_props: dict or None
            a dict of the read-in yaml file specifications for this
            field. None if none were found.

        verbose: bool
            If true, be talkative about what we find.

        recursion_level: int
            Current recursion level. Used when handling structs
            defined within the parent struct and unions as an emergency
            exit in case something goes wrong. Also used to determine the
            indentation level of the contents of the sub-struct/union contents.

        """

        if recursion_level > 20:
            raise ValueError("We recursed 20 times, this seems excessive.")

        self.name = field_name
        self.props = field_props
        self.verbose = verbose
        self.recursion_level = recursion_level

        # initialise default values
        self.type = "float"
        self.size = 1
        self.documentation = None
        self.sub_entries = []
        self.ifdef = None
        self.ifdef_return_val = None

        # Now read what was written.
        self._extract_field_data()
        return

    def _extract_field_data(self):
        """
        Extracts the field data from the field_props dict.
        """

        if self.name == "union":
            # Change the internal name of the union so it has a unique key in
            # the dict that we'll create
            self.name = "union" + str(self.union_count)
            self.union_count += 1
            self.type = "union"

        if self.verbose:
            print(f"Found field '{self.name}'")

        if self.props is None:
            if self.type == "union":
                raise ValueError("Found 'union' with no further specification?")
            if self.type == "struct":
                raise ValueError("Found 'struct' with no further specification?")
            if self.verbose:
                # default is set in initialiser
                print(f"  type: default [{self.type}]")

            # nothing else to do.
            return

        # We have props to sort through.
        # First, validate that we have no unknown fields.
        for key in self.props.keys():
            if key not in _allowed_field_descriptors:
                print(f"WARNING: Field '{self.name}':", file=sys.stderr)
                print(
                    f"WARNING: Unknown field descriptor '{key}'. Skipping it.",
                    file=sys.stderr,
                )

        # Now see what's around.
        try:
            self.type = self.props["type"]
            if self.verbose:
                print("  type:", self.type)
        except KeyError:
            pass

        if self.type not in _allowed_field_data_types:
            if not (self.type.startswith("struct ") or self.type.endswith("*")):
                raise ValueError(f"Unknown data type '{self.type}'")

        try:
            self.size = self.props["size"]
            if self.verbose:
                print("  size:", self.size)
        except KeyError:
            pass

        try:
            self.documentation = self.props["doc"]
            if self.verbose:
                print("  documentation:", self.documentation)
        except KeyError:
            pass
        try:
            self.documentation = self.props["documentation"]
            if self.verbose:
                print("  documentation:", self.documentation)
        except KeyError:
            pass
        try:
            self.ifdef = self.props["ifdef"]
            if self.verbose:
                print("  macro guard:", self.ifdef)
        except KeyError:
            pass
        try:
            self.ifdef_return_val = self.props["ifdef_return_val"]
            if self.verbose:
                print("  getter return value:", self.ifdef_return_val)
        except KeyError:
            pass

        if self.type == "struct" or self.type == "union":
            try:
                contents = self.props["contents"]
            except KeyError:
                raise KeyError(
                    f"'{self.type} {self.name}' definition within parent struct needs 'contents' specification."
                )
            # recursively read in sub-components of this struct or union
            for key in contents.keys():
                new_entry = FieldEntry(
                    key,
                    contents[key],
                    verbose=self.verbose,
                    recursion_level=self.recursion_level + 1,
                )
                self.sub_entries.append(new_entry)

        #  if self.type == "struct" or self.type.startswith("struct "):
        if self.type == "struct":
            if self.ifdef != None:
                # we usually use empty structs instead in SWIFT, i.e. their
                # contents are hidden behind macro guards. The problem we face
                # here is to decide how to define a default return value for
                # the getters.
                raise NotImplementedError(
                    f"'{self.type} {self.name}': structs hidden behind macro guards not implemented"
                )

        return

    def _get_field_dict(
        self,
        parent_struct: Union[str, None] = None,
        indent_level: int = 0,
        verbose: bool = False,
    ) -> dict:
        """
        Generate a dict with this field's properties for the jinja templates.

        Parameters
        ----------

        parent_struct: None or str
            If not None, name of the struct this field is internal to.

        indent_level: int
            Increment the indent level of the declaration. Each indent_level
            indents the line by 2 whitespaces.

        verbose: bool
            Set verbosity

        Returns
        -------

        d: dict
            dict containing field properties prepared for use in jinja templates
        """

        is_pointer = False
        if "*" in self.type:
            # don't make pointers const
            is_pointer = True

        if self.ifdef:
            # do we use the default return value?
            if self.ifdef_return_val is None:
                if is_pointer:
                    self.ifdef_return_val = _field_data_type_default_return_vals[
                        "pointer"
                    ]
                else:
                    self.ifdef_return_val = _field_data_type_default_return_vals[
                        self.type
                    ]
            # if still None, something is wrong.
            if self.ifdef_return_val is None:
                raise ValueError(
                    "No return value available for field with IFDEF."
                    + f"name: {self.name}, type: {self.type}"
                )

        d = {
            "NAME": self.name,
            "SIZE": self.size,
            "DOC": self.documentation,
            "TYPE": self.type,
            "INDENT_LEVEL": indent_level,
            "IFDEF": self.ifdef,
            "IFDEF_RETURN_VAL": self.ifdef_return_val,
            "PARENT_STRUCT": parent_struct,
            "IS_POINTER": is_pointer,
            "HAS_DOC": self.documentation is not None,
            "HAS_IFDEF": self.ifdef is not None,
            "HAS_PARENT_STRUCT": parent_struct is not None,
            "IS_ARRAY": self.size > 1,
            "IS_UNION": self.type == "union",
            "IS_INTERNAL_STRUCT": self.type == "struct",
        }

        if verbose:
            print("----", d)

        return d

    def generate_declaration(self, indent_level: int = 1, verbose: bool = False):
        """
        Generate the field declaration

        indent_level: int
            Increment the indent level of the declaration. Each indent_level
            indents the line by 2 whitespaces.

        verbose: bool
            Set verbosity level
        """

        if verbose:
            print(f"-- Generating declaration for {self.type} {self.name}")

        params_dict = self._get_field_dict(indent_level=indent_level, verbose=verbose)

        if params_dict["IS_INTERNAL_STRUCT"] or params_dict["IS_UNION"]:
            decl_subfields = ""
            for entry in self.sub_entries:
                new_entry_c_code = entry.generate_declaration(indent_level + 1)
                decl_subfields = "".join((decl_subfields, new_entry_c_code))
            params_dict["SUB_FIELD_DECLARATIONS"] = decl_subfields
            templ = self.jinja_env.get_template(
                "struct_field_declaration.jinja.template"
            )
            decl = templ.render(params_dict)

        else:
            templ = self.jinja_env.get_template("field_declaration.jinja.template")
            decl = templ.render(params_dict)

        return decl

    def generate_API(
        self, parent_struct: Union[str, None] = None, verbose: bool = False
    ) -> str:
        """
        Generate the getters and setters C code.

        Parameters
        ----------

        parent_struct: str or None
            If not None, this denoted that field is a sub-field contained within
            the given parent struct.

        verbose: bool
            Set verbosity level

        Returns
        -------

        api: str
            generated API C-code as a string
        """

        if verbose:
            print(f"-- Generating API for {self.type} {self.name}")

        params_dict = self._get_field_dict(parent_struct=parent_struct, verbose=verbose)

        if params_dict["IS_INTERNAL_STRUCT"] or params_dict["IS_UNION"]:
            api_sub_entries = []
            for entry in self.sub_entries:
                if params_dict["IS_UNION"]:
                    # we don't need any modification to the accessors.
                    api_entry = entry.generate_API(verbose=verbose)
                else:
                    parent = parent_struct
                    if parent_struct is not None:
                        # Append struct recursively
                        parent = parent_struct + "._" + self.name
                    else:
                        parent = self.name
                    api_entry = entry.generate_API(
                        parent_struct=parent, verbose=verbose
                    )

                api_sub_entries.append(api_entry)

            api = "".join(api_sub_entries)
        else:
            if params_dict["IS_ARRAY"]:
                if params_dict["IS_POINTER"]:
                    raise NotImplementedError(
                        f"Field {self.type} {self.name}[{self.size}]:"
                        + "arrays of pointers is untested, could contain errors."
                    )
                templ = self.jinja_env.get_template("api_array.jinja.template")
            else:
                templ = self.jinja_env.get_template("api_scalar.jinja.template")

            api = templ.render(params_dict)

        return api
