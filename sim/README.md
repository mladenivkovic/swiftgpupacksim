swiftgpupacksim
===============

Runs the actual simulation of packing/unpacking.


Building the program
--------------------

1) Run `./autoreconf --install --symlink --force` or simply `./autogen.sh`

2) Run `./configure`.
   The interesting options are:

  ```
  --enable-no-cuda-mallochost
                            Disable cudaMallocHost calls and use malloc instead
                            [yes/no]
  --enable-vectorization-pragmas
                            Enable manually encouraged vectorization through
                            simd pragmas (default: no) [yes/no]
  --with-particle-memory-layout=<method>
                          Particle memory layout to use [aos, soa,
                          soa-modified, soa-manual, upstream, pack-gradient,
                          pack-force, pack-shared, default: aos]
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

