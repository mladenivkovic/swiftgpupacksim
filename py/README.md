# Particle Struct Generation Scripts

This directory contains the scripts used to generate the various realisations of
particle structs.

Documentation to be updated as script grows.

For now, run e.g. `./generate_hydro_part.py input/SPHENIX.yml`.
Use

```
py generate_hydro_part.py --help
```

to see all available cmdline options.


Dependencies:
- pyyaml
- jinja2



## Yaml syntax

The structs are generated using specifications in yaml files.
Some defaults are stored in `input/`.

### Scalar and array native data types

The general specification requirement is as follows:


```
part_struct1_name:          # name of struct (e.g. 'part', 'density', 'force'...)
  field1_name:              # name of particle field (e.g. 'x', 'm', 'h'...)
    type: DATA_TYPE         # Optional: C data type of the field. E.g. float,
                            # double, int, long long, timebin_t...
                            # If not provided, defaults to 'float'.
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



### Structs

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

    /*! the first field of 'your_struct_name' struct
    int a;

    /*! the second field of 'your_struct_name' struct
    double b;

  } your_struct_name;
};

/* getters and setters... */

```

To use a struct which is defined somewhere else, just add it as a type
descriptor. Example:

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

**WARNING**: This currently doesn't work for structs or unions.
