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

## Configuration options

### Development options

- `--enable-debug`: Enable debugging symbols
- `--enable-debugging-checks`: Enable additional debugging checks
- `--enable-no-cuda-mallochost`: Use `malloc` instead of `cudaMallocHost`
- `--enable-sanitizer`, `--enable-undefined-sanitizer`: Turn on sanitizers

### Selecting a memory layout, alignments, loop splitting, particle access variant

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


## Reading the output

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



## Adding new measurement data

The measured event logs are kept under [git-lfs]() revision. To add your new
data correctly without messing up this repo's size, do the following:

- Make a new empty subdirectory to `data/`
- Add the path to that subdirectory in `.gitattributes`
- Move your data into your new subdir
- `git add` your data and `.gitattributes` to stage them
- Make a `git commit` as usual.

Consult `/data/README.md` for a specification of the log file formats.




## Adding new layout realisations

TODO

## Particle struct generation

The purpose of this mini-benchmark is to try out various particle memory layout
realisations. To do so, we can generate different realisations of particle
structs using `python 3` and the `jinja2` templating engine. Consult the
`py/README.md` for instructions how to use the scripts.


