# swiftgpupacksim

Mini-application to experiment with memory layout strategies and benchmark
SWIFT's gpu packing and unpacking procedures.

The main idea is to log the execution order of GPU packing and unpacking
tasks during a run with SWIFT and then to use the mini-benchmark contained in
this repositody to re-run the same order using varying memory layouts, hoping to
find a better/optimal solution.

The logging/recording of tasks is not included in this repository. For that
check out the [`log_packing` branch of the GPU repository](https://github.com/abouzied-nasar/SWIFT/tree/log_packing).

Some collected data should be available in the `data/` directory of this
repository.



## Instructions

The experiment run can be created in the `/bin` directory.

There are some pre-defined compilers & flags for different machines and setups
stored in the `bin/config_*.mk` files. Select the one you like, or add your own,
by including the corresponding file at the top of `bin/Makefile`.

Run `make debug` to switch compilation to debug mode. Run `make nodebug` to
switch it off again.

Run the compiled `swiftgpupacksim` executable with the path to the data to be
read in as the only cmdline arg to execute the simulation, e.g.

```
./swiftgpupacksim ../data/IntelXeonGold512
```

