# swiftgpupacksim

Mini-application to experiment with memory layout strategies and benchmark
SWIFT's gpu packing and unpacking procedures.

The main idea is to record the execution order of GPU packing and unpacking
tasks during a run with SWIFT and then to use this mini-benchmark to re-run the
same order using varying memory layouts.


## instructions

The experiment run can be created in the `/bin` directory.

There are some pre-defined compilers & flags for different machines and setups
stored in the `bin/config_*.mk` files. Select the one you like, or add your own,
by including the corresponding file at the top of `bin/Makefile`.

Run `make debug` to switch compilation to debug mode. Run `make nodebug` to
switch it off again.



