
Note: This readme is probably best read in a markdown reader or
[on github](https://github.com/mladenivkovic/swiftgpupacksim/blob/main/README.md)
rather than in plain text.


swiftgpupacksim
===============

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
to be made available (see the [instructions](#getting-sample-experiment-data) below).



Installation
------------

Building the mini-app
~~~~~~~~~~~~~~~~~~~~~

The mini-benchmark is stored in the `/sim` subdirectory. To install it, run
```
cd sim
./autogen.sh    # or just autoreconf --install --symlink --force
./configure
make
```

See `sim/README.md` for more details on configuration options.

This installation requires `autotools`, `libtool`, and `make` (same
requirements as SWIFT).

Successful compilation will leave you with the executable `/sim/swiftgpupack`.
To run it, you need to provide it with logged experiment data to read in and
reproduce. Run it using

```
./sim/swiftgpupack path/to/experiment/logs
```

Some example scripts are given in `/sim/examples`.


Getting sample experiment data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some experiment data (logs of packing/unpacking operations on actual GPU runs)
are stored in the `data/` directory. However, they are stored via `git-lfs`. To
access them after cloning this repository, use

```
git lfs fetch --all
git lfs checkout
```

In case you want to convert the large files back to file pointers due to e.g.
limited disk space, use

```
git read-tree HEAD && GIT_LFS_SKIP_SMUDGE=1 git checkout -f HEAD
```

WARNING: If you run that in a dirty working tree, you'll blow away your
changes, so don't do that.


Getting the `swiftpartgen` submodule
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[swiftpartgen](https://github.com/SWIFTSIM/swiftpartgen) is used to generate
header files for various particle layouts and their respective getter/setter
APIs. If you have need of that, `swiftpartgen` is part of this repository as
a submodule. To fetch it, run

```
git submodule init
git submodule update
```





Configuration options
---------------------

See `sim/README.md` for more details.

Development options
~~~~~~~~~~~~~~~~~~~

- `--enable-debug`: Enable debugging symbols
- `--enable-debugging-checks`: Enable additional debugging checks
- `--enable-no-cuda-mallochost`: Use `malloc` instead of `cudaMallocHost`
- `--enable-sanitizer`, `--enable-undefined-sanitizer`: Turn on sanitizers



Selecting a memory layout, alignments, loop splitting, particle access variant
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See `/sim/README.md` for more details.

To select a specific memory layout, use the
`--with-particle-memory-layout=<method>` flag in the `./configure` step. See
`./configure --help` for available options.

You can set the struct alignments and particle array alignments using the
`--with-struct-align=<value>` and `--with-part-align=<value>` flags.

Particle access variants (how particle data is being accessed via
getters/setters) can be selected using the `--with-particle-access=<value>` flag.

Loop splitting variants (i.e. splitting the loop iterating over particle data to
pack them into buffers in multiple loops instead of a single iteration per
particle) can be selected using the `--with-loop-split=<value>` flag.




Reading the output
~~~~~~~~~~~~~~~~~~

A simulation run will produce an output file of the form `results_XXX.csv`,
where `XXX` is determined by the selected memory layout realisation.
The contents should be the following:

```
# nr_threads: [INT]
# nr_parts: [INT]
# nr_steps: [INT]
# cache flush: 0 or 1
# stuct align: [INT]
# part array align: [INT]
# memory layout: XXX [aos, soa, pack_gradient, pack_force, pack_shared]
# access variant: XXX [part-struct, global-var, explicit-var]
# loop splitting variant: [none, by-struct, by-element]
# vectorization: 0 or 1
# packed structs: 0 or 1
# reproduced measurements: path/to/root/dir/of/measurements
# type,subtype,timing[ms]
pack,density,[FLOAT]
pack,gradient,[FLOAT]
pack,force,[FLOAT]
unpack,density,[FLOAT]
unpack,gradient,[FLOAT]
unpack,force,[FLOAT]
```



Adding new measurement data
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The measured event logs are kept under [git-lfs]() revision. To add your new
data correctly without messing up this repo's size, do the following:

- Make a new empty subdirectory to `data/`
- Add the path to that subdirectory in `.gitattributes`
- Move your data into your new subdir
- `git add` your data and `.gitattributes` to stage them
- Make a `git commit` as usual.

Consult `/data/README.md` for a specification of the log file formats.




Adding new layout realisations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

New layouts are easiest implemented using [swiftpartgen](https://github.com/SWIFTSIM/swiftpartgen).
It is kept as a git submodule in this repository. To fetch it, use

```
git submodule init
git submodule update
```

Consult [its documentation](https://swiftsim.github.io/swiftpartgen/) on how to
use it. In short, you'll have to:

- write a `yaml` parameter file specifying your new layout
- run the `swiftpartgen.py` script
- add the resulting output to a new subdirectory in `sim/src/realisations`
- add a new entry for the layout in `sim/configure.ac`

and that should hopefully be it!



