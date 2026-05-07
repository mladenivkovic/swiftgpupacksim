#!/usr/bin/env python3

import argparse
import numpy as np
import os

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
    Read in all packing/unpacking logs from a directory and print statistics to screen.

    Log data are expected to have format:
      log_threadXXX_stepYYY.dat
    """,
)
parser.add_argument("srcdir", help="directory to read in")
parser.add_argument(
        "-v",
    "--verbose",
    dest="verbose",
    action="store_true",
    help="Be more talkative",
)

args = parser.parse_args()
verbose = args.verbose
srcdir = args.srcdir



if not os.path.exists(srcdir):
    raise FileNotFoundError(f"{srcdir} not found.")

ls = os.listdir(srcdir)

# First find number of steps and number of threads
fthreadcount = []
fstepcount = []
for f in ls:
    if f.startswith("log_thread000") and f.endswith(".dat"):
        fstepcount.append(f)
    if f.startswith("log_thread") and f.endswith("_step000.dat"):
        fthreadcount.append(f)

nsteps = len(fstepcount)
nthreads = len(fthreadcount)

print(f"Found {nthreads} threads and {nsteps} steps")


subtypes_all = None
types_all = None
counts_all = None

for step in range(nsteps):

    if verbose: print("Running step", step)

    subtypes = None
    types = None
    counts = None

    for thread in range(nthreads):
        if verbose: print("Running thread", thread)

        logfile = f"log_thread{thread:03}_step{step:03}.dat"
        fulllogfile = os.path.join(srcdir, logfile)
        subtype, pou, count = np.genfromtxt(fulllogfile,
                                         dtype = ["<U1", "<U1", np.int32, np.int32, np.int32, np.float32], # "<U1": Unicode character <= 1 character in length
                                         comments="/",
                                         delimiter=",",
                                         #  usecols=[0,1,2,3,4,5], # ignore timing
                                         usecols=[0,1,3,], # ignore timing
                                         unpack=True,
                             )

        # aggregate data over all threads
        if subtypes is None:
            subtypes = np.copy(subtype)
        else:
            subtypes = np.concatenate((subtypes, subtype))
        if types is None:
            types = np.copy(pou)
        else:
            types = np.concatenate((types, pou))
        if counts is None:
            counts = np.copy(count)
        else:
            counts = np.concatenate((counts, count))


    n_density_pack = np.count_nonzero(types[subtypes == 'd'] == 'p')
    n_density_unpack = np.count_nonzero(types[subtypes == 'd'] == 'u')
    n_gradient_pack = np.count_nonzero(types[subtypes == 'g'] == 'p')
    n_gradient_unpack = np.count_nonzero(types[subtypes == 'g'] == 'u')
    n_force_pack = np.count_nonzero(types[subtypes == 'f'] == 'p')
    n_force_unpack = np.count_nonzero(types[subtypes == 'f'] == 'u')

    counts_sorted = np.sort(counts)


    print()
    print(f"Step {step}")
    print("-----------")
    print( "              pack     unpack")
    print(f"Density:      {n_density_pack:<8d} {n_density_unpack:<8d}")
    print(f"Gradient:     {n_gradient_pack:<8d} {n_gradient_unpack:<8d}")
    print(f"Force:        {n_force_pack:<8d} {n_force_unpack:<8d}")
    print()
    print(f"                   min  max  mean     median")
    print(f"# parts in cells   {counts.min():<4d} {counts.max():<4d} {counts.mean():<8.3f} {counts_sorted[counts.size // 2]:<4d}")
    print()


    # Store aggregate data for full simulation
    if subtypes_all is None:
        subtypes_all = np.copy(subtypes)
    else:
        print(subtypes_all.shape)
        print(subtypes.shape)
        subtypes_all = np.concatenate((subtypes_all, subtypes))
    if types_all is None:
        types_all = np.copy(types)
    else:
        types_all = np.concatenate((types_all, types))
    if counts_all is None:
        counts_all = np.copy(counts)
    else:
        counts_all = np.concatenate((counts_all, counts))


n_density_pack = np.count_nonzero(types_all[subtypes_all == 'd'] == 'p')
n_density_unpack = np.count_nonzero(types_all[subtypes_all == 'd'] == 'u')
n_gradient_pack = np.count_nonzero(types_all[subtypes_all == 'g'] == 'p')
n_gradient_unpack = np.count_nonzero(types_all[subtypes_all == 'g'] == 'u')
n_force_pack = np.count_nonzero(types_all[subtypes_all == 'f'] == 'p')
n_force_unpack = np.count_nonzero(types_all[subtypes_all == 'f'] == 'u')

counts_all_sorted = np.sort(counts_all)

print()
print(f"Full Logs")
print("-----------")
print( "              pack     unpack")
print(f"Density:      {n_density_pack:<8d} {n_density_unpack:<8d}")
print(f"Gradient:     {n_gradient_pack:<8d} {n_gradient_unpack:<8d}")
print(f"Force:        {n_force_pack:<8d} {n_force_unpack:<8d}")
print()
print(f"                   min  max  mean     median")
print(f"# parts in cells   {counts.min():<4d} {counts.max():<4d} {counts.mean():<8.3f} {counts_sorted[counts.size // 2]:<4d}")
print()



