# Particle Struct Generation Scripts

This directory contains the scripts used to generate the various realisations of
particle structs.

Documentation to be updated as script grows.

For now, run `./generate_hydro_part.py input/default.yml`

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
    type: DATA_TYPE         # Optional: field data type. E.g. float, double, int, long long, ...
                            # If not provided, defaults to 'float'.
    size: N                 # Optional. If > 1, field is assumed to be fixed size array.
    doc: documentation text # Optional. Documents particle data field.

  # some more concrete examples.
  x:
    type: double
    size: 3
    doc: the particle position
  m: # no further specification works too.
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


### Unions

To specify a union within a parent particle struct, use the following
syntax:

```
parent_struct:         # such as part, xpart, ...
  union:
    contents:
      a:
        type: int
        documentation: "the first field of union"
      b:
        type: double
        documentation: "the second field of union"
```





