# swiftgpupacksim

Mini-application to experiment with memory layout strategies and benchmark
SWIFT's gpu packing and unpacking procedures.

The main idea is to log the execution order of GPU packing and unpacking tasks
during a run with SWIFT and then to use the mini-benchmark contained in this
repositody to re-run the same order using varying memory layouts, hoping to
find a better/optimal solution.

The logging/recording of tasks is not included in this repository. For that
check out the
[`log_packing`branch of the GPU repository](https://github.com/abouzied-nasar/SWIFT/tree/log_packing).
See also the logging format specification in `data/REAMDE.md`.

Some collected data should be available in the `data/` directory of this
repository. Note that it's stored using `git lfs` and needs to be checked out
to be made available (see the [instructions](#Instructions) below).



## Instructions

The mini-benchmark is stored in the `/sim` subdirectory. To install it, run
```
cd sim
./autogen.sh    # or just autoreconf --install --symlink --force
./configure
make
```

This installation requires `autotools`, `libtool`, and `make` (same
requirements as SWIFT).

Successful compilation will leave you with the executable `/sim/swiftgpupack`.
To run it, you need to provide it with logged experiment data to read in and
reproduce. Run it using

```
./sim/swiftgpupack path/to/experiment/logs
```

Some example scripts are given in `/sim/examples`.

Some experiment data (logs of packing/unpacking operations on actual GPU runs)
are stored in the `data/` directory. However, they are stored via `git-lfs`. To
access them after cloning this repository, use

```
git lfs fetch --all
git lfs checkout
```





## Adding new measurement data

The measured event logs are kept under [git-lfs]() revision. To add your new
data correctly without messing up this repo's size, do the following:

- Make a new empty subdirectory to `data/`
- Add the path to that subdirectory in `.gitattributes`
- Move your data into your new subdir
- `git add` your data and `.gitattributes` to stage them
- Make a `git commit` as usual.

Consult `/data/README.md` for a specification of the log file formats.





## Particle struct generation

The purpose of this mini-benchmark is to try out various particle memory layout
realisations. To do so, we can generate different realisations of particle
structs using `python 3` and the `jinja2` templating engine. Consult the
`py/README.md` for instructions how to use the scripts.


