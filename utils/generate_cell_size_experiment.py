#!/usr/bin/env python3

import os
import argparse
import random

random.seed(666)

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="""
    Generate mock logs of an experiment for a given "cell size", i.e.
    particle count per cell.
    """,
)
parser.add_argument(
    "-n",
    "--nparts",
    nargs=1,
    dest="nparts",
    help="Particle number per cell",
    type=int
)
parser.add_argument(
    "-t",
    "--total-parts",
    nargs=1,
    dest="nparts_tot",
    help="Total number of particles",
    default=10000000,
    type=int
)
parser.add_argument(
    "-l",
    "--n-logs",
    nargs=1,
    dest="n_logs",
    help="Number of logs to generate for each pack/unpack operation type. (Total number of logs will be this multiplied by 6)",
    default=200000,
    type=int
)
parser.add_argument(
    "-b",
    "--buffer-size",
    nargs=1,
    dest="buffer_size",
    help="Size of GPU buffer arrays to use",
    default=1000000,
    type=int
)

args = parser.parse_args()
nparts = args.nparts
if isinstance(nparts, list):
    nparts = nparts[0]
nparts_tot = args.nparts_tot
if isinstance(nparts_tot, list):
    nparts_tot = nparts_tot[0]
n_logs = args.n_logs
if isinstance(n_logs, list):
    n_logs = n_logs[0]
buffer_size = args.buffer_size
if isinstance(buffer_size, list):
    buffer_size = buffer_size[0]


outdir = f"TestCellSize_{nparts}"
if not os.path.isdir(outdir):
    os.mkdir(outdir)

# First write parameter file
log_params_file = os.path.join(outdir, "log_runtime_params.dat")
log_params_fp = open(log_params_file, "w")
log_params_fp.write("nr_threads: 1\n")
log_params_fp.write(f"nr_parts: {nparts_tot}\n")
log_params_fp.write(f"nr_steps: 1\n")
log_params_fp.close()

# We do this once and re-use this list for all 6 operation types.

# highest index we can start operating on
max_start_ind = nparts_tot - nparts - 1

# Generate a list of all operations.
log_indexes = []
for i in range(n_logs):
    start_ind = random.randint(0, max_start_ind)
    log_indexes.append(start_ind)
    if start_ind > max_start_ind:
        raise ValueError("Huh?")

prefixes = ["d,p", "d,u", "g,p", "g,u", "f,p", "f,u"]
log_starts = []
for i in range(n_logs):
    for pref in prefixes:
        log_starts.append(f"{pref},{log_indexes[i]},{nparts}")

# randomly sort order of operations
random.shuffle(log_starts)

# now get buffer indexes
log_full = []
c_dp = 0
c_du = 0
c_gp = 0
c_gu = 0
c_fp = 0
c_fu = 0

for line in log_starts:
    if line.startswith("d,p"):
        c_use = c_dp
        c_dp += nparts
        if c_dp >= buffer_size:
            c_dp = 0
            c_use = 0
    elif line.startswith("d,u"):
        c_use = c_du
        c_du += nparts
        if c_du >= buffer_size:
            c_du = 0
            c_use = 0
    elif line.startswith("g,p"):
        c_use = c_gp
        c_gp += nparts
        if c_gp >= buffer_size:
            c_gp = 0
            c_use = 0
    elif line.startswith("g,u"):
        c_use = c_gu
        c_gu += nparts
        if c_gu >= buffer_size:
            c_gu = 0
            c_use = 0
    elif line.startswith("f,p"):
        c_use = c_fp
        c_fp += nparts
        if c_fp >= buffer_size:
            c_fp = 0
            c_use = 0
    elif line.startswith("f,u"):
        c_use = c_fu
        c_fu += nparts
        if c_fu >= buffer_size:
            c_fu = 0
            c_use = 0

    fullline = f"{line},{c_use},0.0\n"
    log_full.append(fullline)

    if c_use + nparts >= buffer_size:
        raise ValueError("Huh?", c_use, buffer_size, line)


# Now write mock log
log_file = os.path.join(outdir, "log_thread000_step000.dat")
log_fp = open(log_file, "w")
log_fp.write("// mock log file generated with /utils/generate_cell_size_experiment.py\n")
log_fp.write("// subtype,pack_or_unpack,c_offset,count,index,time\n")
for line in log_full:
    log_fp.write(line)
log_fp.close()

print(f"Written outputs to {outdir}")

