#!/usr/bin/env python3

import yaml
from typing import Union
from templates import init_jinja_env


# List of all permissible field descriptors for a particle field.
# Used for consistency checks.
_allowed_field_descriptors = [
    "type",
    "size",
    "doc",
    "documentation",
    "contents"
]

# List of all permissible particle field data types.
# Used for consistency checks.
_allowed_field_data_types = [
    "int",
    "long",
    "long long",
    "float",
    "double",
    "struct"
]


class FieldEntry(object):
    """
    A class to store a particle field entry from the yaml
    input files.
    """

    # jinja environment used to load and generate templates
    jinja_env = init_jinja_env()

    def __init__(self, field_name: str, field_props: Union[dict, None], verbose=False, recursion_level=0):
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

        # Now read what was written.
        self._extract_field_data()
        return

    def _extract_field_data(self):
        """
        Extracts the field data from the field_props dict.
        """

        if self.verbose:
            print(f"Found field '{self.name}'")

        if self.props is None:
            if self.verbose:
                # default is set in initialiser
                print(f"  type: default [{self.type}]")

            #nothing else to do.
            return

        # We have props to sort through.
        # First, validate that we have no unknown fields.
        for key in self.props.keys():
            if key not in _allowed_field_descriptors:
                print(f"WARNING: Field '{self.name}':")
                print(f"WARNING: Unknown field descriptor '{key}'. Skipping it.")

        # Now see what's around.
        try:
            self.type = self.props["type"]
            if self.verbose:
                print("  type:", self.type)
        except KeyError:
            pass

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

        # todo: struct* ?
        if self.type == "struct":
            try:
                contents = self.props["contents"]
            except KeyError:
                raise KeyError("'struct' definition within struct needs contents.")
            for key in contents.keys():
                new_entry = FieldEntry(key, contents[key], verbose=self.verbose, recursion_level=self.recursion_level + 1)
                self.sub_entries.append(new_entry)

        return


    def _get_field_dict(self, indent_level: int = 0) -> dict:
        """
        Generate a dict with this field's properties for the jinja templates.

        indent_level: int
            Increment the indent level of the declaration. Each indent_level
            indents the line by 2 whitespaces.
        """

        d = {
            "NAME": self.name,
            "SIZE": self.size,
            "DOC": self.documentation,
            "TYPE": self.type,
            "INDENT_LEVEL": indent_level,
            "HAS_DOC": self.documentation is not None,
            "IS_ARRAY": self.size > 1,
            "IS_UNION": self.type == "union",
            "IS_STRUCT": self.type == "struct",
        }

        return d

    def generate_declaration(self, indent_level: int = 1):
        """
        Generate the field declaration

        indent_level: int
            Increment the indent level of the declaration. Each indent_level
            indents the line by 2 whitespaces.
        """

        params_dict = self._get_field_dict(indent_level)

        if params_dict["IS_STRUCT"] or params_dict["IS_UNION"]:
            decl_subfields = ""
            for entry in self.sub_entries:
                new_entry_c_code = entry.generate_declaration(indent_level + 1)
                decl_subfields = "".join((decl_subfields, new_entry_c_code))
            params_dict["SUB_FIELD_DECLARATIONS"]=decl_subfields
            templ = self.jinja_env.get_template("struct_field_declaration.jinja.template")
            decl = templ.render(params_dict)

        else:
            templ = self.jinja_env.get_template("field_declaration.jinja.template")
            decl = templ.render(params_dict)

        return decl

    def generate_API(self):
        """
        Generate the getters and setters C code.
        """

        params_dict = self._get_field_dict()
        if params_dict["IS_ARRAY"]:
            templ = self.jinja_env.get_template("api_array.jinja.template")
        else:
            templ = self.jinja_env.get_template("api_scalar.jinja.template")

        api = templ.render(params_dict)
        return api
