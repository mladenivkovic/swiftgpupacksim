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
]

# List of all permissible particle field data types.
# Used for consistency checks.
_allowed_field_data_types = [
    "int",
    "long",
    "long long",
    "float",
    "double",
]


class FieldEntry(object):
    """
    A class to store a particle field entry from the yaml
    input files.
    """

    # jinja environment used to load and generate templates
    jinja_env = init_jinja_env()

    def __init__(self, field_name, field_props: Union[dict, None], verbose=False):

        self.field_name = field_name
        self.field_props = field_props
        self.verbose = verbose

        # initialise default values
        self.field_type = "float"
        self.field_size = 1
        self.documentation = None

        # Now read what was written.
        self._extract_field_data()
        return

    def _extract_field_data(self):
        """
        Extracts the field data from the field_props dict.
        """

        if self.verbose:
            print(f"Found field '{self.field_name}'")

        if self.field_props is None:
            if self.verbose:
                # default is set in initialiser
                print(f"  type: default [{self.field_type}]")
        else:
            # first, validate that we have no unknown fields.
            for key in self.field_props.keys():
                if key not in _allowed_field_descriptors:
                    print(f"WARNING: Field '{self.field_name}':")
                    print(f"WARNING: Unknown field descriptor '{key}'. Skipping it.")
            # Now see what's around.
            try:
                self.field_type = self.field_props["type"]
                if self.verbose:
                    print("  type:", self.field_type)
            except KeyError:
                pass

            try:
                self.field_size = self.field_props["size"]
                if self.verbose:
                    print("  size:", self.field_size)
            except KeyError:
                pass

            try:
                self.documentation = self.field_props["doc"]
                if self.verbose:
                    print("  documentation:", self.documentation)
            except KeyError:
                pass
            try:
                self.documentation = self.field_props["documentation"]
                if self.verbose:
                    print("  documentation:", self.documentation)
            except KeyError:
                pass
        return

    def _get_field_dict(self, indent_level: int = 0) -> dict:
        """
        Generate a dict with this field's properties for the jinja templates.

        indent_level: int
            Increment the indent level of the declaration. Each indent_level
            indents the line by 2 whitespaces.
        """

        d = {
            "NAME": self.field_name,
            "SIZE": self.field_size,
            "DOC": self.documentation,
            "TYPE": self.field_type,
            "INDENT_LEVEL": indent_level,
            "HAS_DOC": self.documentation is not None,
            "IS_ARRAY": self.field_size > 1,
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
