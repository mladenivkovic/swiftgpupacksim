# Particle Struct Generation Scripts

This directory contains the scripts used to generate the various C header files
containint the realisations of particle structs used in SWIFT.

Documentation to be updated as script grows.




## Usage

For now, run e.g.

```
./generate_hydro_part.py --global-var-accessor input/SPHENIX.yml
```
to generate files for `swiftgpupacksim`, or

```
./generate_hydro_part.py --global-var-accessor --swift input/SPHENIX.yml
```
to generate files for use in SWIFT.

Use

```
py generate_hydro_part.py --help
```

to see all available cmdline options.

By default, this script will create files used by `swiftgpupacksim`. If you want
to produce header files intended for use in the `swift` code, use the `--swift`
flag.

As input, the script requires a `yaml` file specifying the particle struct and
field layouts. Some are provided in the `input/` directory. For further
specification, see the [documentation below](#yaml-particle-specification-file).


Look at the `./make_all_sphenix_variations.sh` script to re-generate all files
used in swiftgpupacksim.
Look at the `./make_swift_particles.sh` script for an example how to generate
particle files for the actual SWIFT codebase, not swiftgpupacksim.


## Accessor Variations

This script offers three variants of accessing the getters/setters API:

- part struct accessors: (`--part-struct-accessors`) getters/setters require a
  `struct part* p` to access correct particle fields. If the field is not
  contained within `struct part` itself, but some other particle sub-struct, it
  will access it using the particle's index in the global particle arrays as
  well as a pointer to all global particle arrays, which are two additional
  variables automatically added to the `struct part`.

- explicit accessors: (`--explicit-var-accessors`) getters/setters require a
  struct which holds pointers to all global particle arrays as well as the
  particle's index in that array to be passed explicitly as arguments. The
  header file containin the definition of the struct holding pointers to all
  global particle arrays is automatically generated too.

- global var accessors: (`--global-var-accessors`) getters/setters only require
  a particle's index in the global particle arrays to access its data. The
  global particle arrays are kept as a global variable somewhere in the code.




## Dependencies

- `pyyaml`
- `jinja2`

Install them e.g. using

```
pip3 install pyyaml jinja2
```



## Installation

No installation needed. The script should run out-of box, provided the
[dependencies](#dependencies) are installed.




## Outputs

- `hydro_part.h`: Header containing a particle struct and its getters/setters
- `part_arrays.h`: Header containing a struct holding all particle data,
  intended for use in cells
- `parts.h`: Header containing generated allocation/freeing of particle array
  data

TODO: there's more now... to be updated when version converges.



# Yaml Particle Specification File

The header files containing particle structs are generated using specifications
in `yaml` files. Some defaults are stored in `input/`.

The specification requires you to provide all particle fields necessary for a
specific particle flavour realisation. The script will then generate header
files required to incorporate those structs into `SWIFT` and `swiftgpupacksim`.

In general, you need to provide at least 2 items:

- a [`metadata` item](#file-metadata), containing file metadata
- one (or arbitrarily many) [`struct`s containing particle
  fields](#particle-structs).

Here's a minimal example:


```
metadata:
  authors: "2026 John Doe"       # year and authors of file, to be inserted into the license
  flavour: my_sph_flavour_name   # name of the SPH flavour. Mainly used in header guard macro

part_struct1_name:          # name of struct (e.g. 'part', 'density', 'force'...)
  field1_name:              # name of particle field (e.g. 'x', 'm', 'h'...)
    type: DATA_TYPE         # Optional: C data type of the field. E.g. float,
                            # double, int, long long, timebin_t...
                            # If not provided, defaults to 'float'.
    size: N                 # Optional. If > 1, field is assumed to be fixed size array.
    doc: documentation text # Optional. Documents particle data field.
```





## File Metadata

The `metadata` item in the `yaml` specification file carries all file metadata,
such as:

- `authors`: Year + author names + emails to be inserted into the GPL3 license
  in the file
- `flavour`: The name of the SPH flavour contained in this file. Mainly used to
  define a header guard and in the file description
- `doc`: File documentation. What will go in the `/** @file` doxygen
  documentation of the file.
- `includes`: list of headers to include. If this field is not provided, a
  default will be used, stored in `input/default_headers_swift.h` and
  `input/default_headers_swiftgpupacksim.h`, respectively.
- `includes_add`: Rather than providing a full list of includes, only add these
  to the default selection.

Here's an example metadata setup:


```
metadata:
  authors: "2026 John Doe"       # year and authors of file, to be inserted into the license
  flavour: my_sph_flavour_name   # name of the SPH flavour. Mainly used in header guard macro
  doc: |                         # file documentation
    This file contains X, Y, and Z.
  includes:                      # headers to be included. If not provided, default headers will
    - a.h                        # be used, read in from `input/default_headers_swift.h` or
    - b.h                        # `input/default_headers_swiftgpupacksim.h`.
    - <stdio.h>
  includes_add:                  # WARNING: Cannot be used together with `includes` parameter.
    - a.h                        # Additional header files to include aside from the "default" ones
    - b.h
    - <stdio.h>
```




## Particle Structs

Your `yaml` specification file must contain at least one particle struct
containing fields, but can contain as many as you like.

A new particle struct containing data is defined by adding a new top-level item
(= item with no indent). To add fields (=actual data) to it, see instructions
below.





### Scalar and Array Native Data Types

The general specification requirement is as follows:


```
metadata:
  authors: "2026 John Doe"       # year and authors of file, to be inserted into the license
  flavour: my_sph_flavour_name   # name of the SPH flavour. Mainly used in header guard macro

part_struct1_name:          # name of struct (e.g. 'part', 'density', 'force'...)
  field1_name:              # name of particle field (e.g. 'x', 'm', 'h'...)
    type: DATA_TYPE         # Optional: C data type of the field. E.g. float,
                            # double, int, long long, timebin_t...
                            # !! If not provided, defaults to 'float'. !!
    size: N                 # Optional. If > 1, field is assumed to be fixed size array.
    doc: documentation text # Optional. Documents particle data field.

  # some more concrete examples.
  x:
    type: double
    size: 3
    doc: the particle position
  m: # no further specification works too. Defaults to float.
  u:
    doc: the comoving specific internal energy
  id:
    type: long long

part_struct2_name:
  field1_name:
    ... # etc etc
```

**IMPORTANT**:

- The top-level entries in the indent hierarchies will be transformed into
  separate structs with that name.
- An exception is the group `metadata`, which should contain the file metadata
  and will not be turned into a struct.
- No duplicate names of particle fields - not even between two different structs!
  - If there are several top level/root particle data structs with identical
    names, the yaml reader will only keep the last one.
  - If there are several fields with identical names, even if they are stored in
    different top level/root particle data structs, this will cause name clashes
    with the generated API.



### Nested Structs

To specify a struct defined within a parent particle struct, use the following
syntax:

```
parent_struct:         # such as part, xpart, ...
  your_struct_name:
    type: struct
    contents:
      a:
        type: int
        documentation: "the first field of 'your_struct_name' struct"
      b:
        type: double
        documentation: "the second field of 'your_struct_name' struct"
```

this results in

```
/* header... */

struct parent_struct {

  struct {

    /*! the first field of 'your_struct_name' struct */
    int a;

    /*! the second field of 'your_struct_name' struct */
    double b;

  } your_struct_name;
};

/* getters and setters... */

```


To use a struct which is defined somewhere else as a data type, just add it as a
type descriptor. Example:


```
parent_struct:         # such as part, xpart, ...
  limiter_data:
    type: struct limiter_timestep_data;
```

results in:

```
parent_struct {
  struct limiter_timestep_data limiter_data;
};
```






### Unions

To specify a union within a parent particle struct, use the following
syntax:

```
parent_struct:         # such as part, xpart, ...
  name_placeholder:
    type: union
    contents:
      a:
        type: int
        documentation: "the first field of union"
      b:
        type: double
        documentation: "the second field of union"
```

The `name_placeholder` name will be unused in the resulting header file.
However, we need a unique identifier for the yaml syntax - otherwise, the
content will be overwritten by the last element with that name and the others
discarded. So just make sure you give them a unique identifier.

This also works with structs as elements of the union.



### enums

Enums (enumerations) are allowed as a data type for field members if they are
defined somewhere else, i.e. this script will not provide a definition for them
in the resulting header files.

Example:

```
part:
  my_enum:
    type: enum color
    doc: some enumeration, defined elsewhere
```

Should result in:

```
struct part {

  /*! some enumeration, defined elsewhere */
  enum color my_enum;

}
```




### IFDEF macros

You may want to hide some particle fields behind a macro guard. To do so, use
the ``ifdef: MACRO_NAME`` keyword.

However, to have functional getters and setters API when the macros aren't set,
we need to provide a return value for the getters. By default, the values will
be set to the max value of the data type (e.g. ``FLT_MAX``, ``INT_MAX``, etc.).
If you want to modify that value, provide the ``ifdef_return_val`` parameter.

Example:

```
part:
  debug_counter:
    type: int
    ifdef: SWIFT_DEBUG_CHECKS

  collect_mpi:
    type: float
    ifdef: WITH_MPI
    ifdef_return_val: 1.f
```

this results in:

```
# ... header and definitions...


/**
 * @brief get debug_counter.
 */
static __attribute__((always_inline)) INLINE int
  part_get_debug_counter(const struct part *restrict p) {
#ifdef SWIFT_DEBUG_CHECKS
  return p->_debug_counter;
#else
  return INT_MAX;
#endif
}

# ... more getters and setters ...

/**
 * @brief get collect_mpi.
 */
static __attribute__((always_inline)) INLINE float
  part_get_collect_mpi(const struct part *restrict p) {
#ifdef WITH_MPI
  return p->_collect_mpi;
#else
  return 1.f;
#endif
}

```

**WARNING**: This currently doesn't work for unions.




### Arrays

To create fields which are fixed-size arrays, add the `size` parameter to the
yaml file. For example:

```
  part:
    v:
      size: 3

    radiation_energy:
      size: RT_NGROUPS

    matrix:
      size: 3,3
```

results in

```
  struct part {

    float v[3];

    float radiation_energy[RT_NGROUPS];

    float matrix[3][3];
  }
```







### Reserved names

To ensure functionality, some names are reserved and cannot be used as names for
fields. These are:

- ``doc``
- ``documentation``
- ``union``
- ``cell_offset``
- ``cell_part_data``



### Split Particle Data Over Multiple Structs

You may want to split the particle data such that they are contained within
multiple structs instead of a single one. To do so, some restrictions apply:

- You *may* define a struct which is named ``part``
  - If you do, it *must* be the first defined struct. TODO: Double-check this,
    it may not be up-to-date any more.
  - If you do, the script will also automatically generate getter functions to
    all other structs holding particle data as defined in the yaml file.
- You *may* not define a struct which is named ``part``.
  - If you don't, one will be generated for you.
  - If you don't, the script will also automatically generate getter functions to
    all other structs holding particle data as defined in the yaml file.


