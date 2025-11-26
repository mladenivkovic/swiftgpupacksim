# Contents

- `IntelXeonGold5218_Gresho64_1thread`: cosma gn001, 64^3 GreshoVortex_3D, single thread



# Data Format

## Run parameter output

Aside from the actual logs (see below), we also need to write out some global
run parameters. The strictly necessary fields are the first thee lines:

```
nr_threads: [INT]   // how many threads this run was performed on
nr_parts:   [INT]   // how many particles we have in the run
nr_steps:   [INT]   // how many steps were recorded
```

This data is written to `log_runtime_params.dat`. Any further data is not
strictly required to re-run the simulation, but can be helpful, such as the git
branch/commit used, the configure and compilation flags, and the output of
`lscpu`.



## logs format

The logging outputs are written per thread and per simulation step, as noted in
the filename of the outputs.

Each log entry is a csv line (that turned out to be smaller data than raw
binary dump):

```
subtype,pack_or_unpack,c_offset,count,index,time
```

where

```
subtype:        d (density), g (gradient), or f (force)
pack_or_unpack: p for packing operation, u for unpacking
c_offset:       offset of c->hydro.parts array in global parts array
count:          c->hydro.count
index:          intex to pack into/unpack from in particle buffer arrays
time:           Measured time for operation, in [micro s]
```

I stored a `git diff` in `data/patch_logging.diff` to track the changes made to
the code used to obtain the logs. You can also check out the
[`log_packing`branch of the GPU repository](https://github.com/abouzied-nasar/SWIFT/tree/log_packing).

