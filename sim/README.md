swiftgpupacksim
===============

Runs the actual simulation of packing/unpacking.


Building the program
--------------------

1) Run `./autoreconf --install --symlink --force` or simply `./autogen.sh`

2) Run `./configure`.
   The interesting options are: (Explanations below)

  ```
  --enable-no-cuda-mallochost
                          Disable cudaMallocHost calls and use malloc instead
                          [yes/no]
  --enable-vectorization-pragmas
                          Enable manually encouraged vectorization through
                          simd pragmas (default: no) [yes/no]
  --enable-packed-structs add __attribute__((packed)) to structs (default: no)
                          [yes/no]
  --with-particle-memory-layout=<method>
                          Particle memory layout to use [aos, soa, upstream,
                          pack-gradient, pack-force, pack-shared, default: aos]
  --with-particle-access=<value>
                          Particle data access variant to use [part-struct,
                          global-var, explicit-var, default: part-struct]
  --with-loop-split=<value>
                          Loop splitting variant to use [none, by-struct,
                          by-element. Default: none]
  --with-struct-align=<value>
                          SWIFT_STRUCT_ALIGNMENT to use [-1, 1, 2, 4, 8, 16,
                          32, 64 default: -1 (=manual (hardcoded) struct
                          align)]
  --with-part-align=<value>
                          SWIFT_PART_ALIGNMENT to use (memalign arrays of
                          particle data) [-1, 1, 32, 64, 128, 256, default: -1
                          (=same as SWIFT_STRUCT_ALIGNMENT)]
  ```





  Interesting developer options:


  ```
  --enable-sanitizer      Enable memory error detection using address
                          sanitizer [no/yes]
  --enable-undefined-sanitizer
                          Enable detection of code that causes undefined
                          behaviour [no/yes]
  --enable-compiler-warnings
                          Enable compile time warning flags, if compiler is
                          known [error/no/yes)]
  --enable-optreport      Enable optimization reports, if compiler is known
                          [error/no/yes)]
  ```



Build variants explanation
--------------------------

- `--with-particle-memory-layout=<method>`
  Particle memory layout to use:
  - `aos`: Array of Structs. One big particle struct in a single global array.
  - `upstream`: Same as `aos`, but the struct fields are arranged the same way
    SWIFT/upstream SPHENIX particle is arranged, whereas `aos` follows the same
    order as the fields are used in the packing and unpacking operations.
  - `soa`: Struct of Arrays. Each particle data field is stored in its own
    array.
  - `pack-gradient`: Particle data is split into several structs. Structs
    correspond to the data fields needed for packing operations (unpacking is
    disadvantaged). Fields needed in both force and gradient packs/unpacks are
    stored in the `gradient` struct.
  - `pack-force`: Same as `pack-gradient`, but fields needed in both force and
    gradient packs/unpacks are stored in the `force` struct.
  - `pack-shared`: Same as `pack-gradient`, but fields needed in both force and
    gradient packs/unpacks are stored in a separate `shared` struct.

- `--with-particle-access=<value>`
  Select particle data access variant, i.e. which getter/setter to use (or: how
  the getter/setters work). Could be passing a `struct part* p` to the getter
  (`part-struct`), could be passing a pointer to the particle data arrays + an
  integer index of where the particle is located in that array (`explicit-var`),
  could be passing just an integer index of a particle and using a globally
  defined variable which is in global scope (`global-var`).

- `--with-loop-split=<value>`
  Manually split the data copying loop into several loops. Either do a single
  loop accessing all of a single particle's data (`none`), or make a new loop
  for each particle data struct on CPU (`by-struct`), or make a new loop for
  each element of the particle data being accessed (`by-element`) (This is
  essentially SoA access).

