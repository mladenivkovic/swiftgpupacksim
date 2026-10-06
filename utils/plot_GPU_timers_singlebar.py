#!/usr/bin/env python3

from swift_hardcoded_data import timer_names

import matplotlib

#  matplotlib.use("Agg")

import argparse
import os
import numpy as np
from matplotlib import pyplot as plt


# Assumed directory structure:
#  `srcdir/node_dir/experiment_dir`
# for all node_dir in node_dirs and for all
# experiment_dir in experiment_dirs

# Set up some defaults

node_dirs = ["gn001", "dine2", "gn002"]
#  node_dirs = ["gn001", "gn002"]
experiment_dirs = ["eagle12", "gresho256"]
experiment_titles = {
        "eagle12": "Eagle12 (highly clustered)",
        "gresho256": "Gresho256 (uniform)",
        }
#  experiment_dirs = ["gresho256"]
timer_file = "timers_0.txt"

node_dir_labels = {
    "gn001": "Intel Xeon Gold 5218 + V100",
    "gn002": "Grace Hopper",
    "dine2": "Intel Xeon Gold 6430 + A30",
    #  "gn002": "Grace Hopper",
    #  "gn001": "Intel+V100",
    #  "dine2": "Intel + A30",
}
# ------------------------------------------------------


# Plot parameters
params = {
    "axes.labelsize": 14,
    "axes.titlesize": 18,
    "font.size": 16,
    "font.family": "serif",
    "legend.fontsize": 12,
    "xtick.labelsize": 16,
    "ytick.labelsize": 16,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.top": True,
    "ytick.right": True,
    "xtick.major.width": 1.5,
    "ytick.major.width": 1.5,
    "axes.linewidth": 1.5,
    "text.usetex": True,
    #  "figure.subplot.left": 0.045,
    #  "figure.subplot.right": 0.99,
    #  "figure.subplot.bottom": 0.05,
    #  "figure.subplot.top": 0.99,
    "figure.subplot.wspace": 0.0,
    #  "figure.subplot.hspace": 0.12,
}
matplotlib.rcParams.update(params)


parser = argparse.ArgumentParser(
    prog="plot_GPU_timers.py",
    description="""
    Plot the outputs of the timers. Provide top level directory to look for
    outputs for as cmdline argument.

    Expected directory hiearchy:
        `srcdir/node_name/experiment_name/`

    Names of node directories and experiment directory names are hardcoded in
    this script.

    By default, it makes 3 plots: One for each task subtype.
    Use -o to plot for each operation (pack, unpack, launch) per plot instead.
    """,
    epilog="""
    To get timers output with swift, first configure with
    `./configure --enable-timers` and then run swift with `./swift --timers`
    """,
)

parser.add_argument("srcdir", help="Top level directory to search for output data")
parser.add_argument(
    "-z",
    "--include-step-zero",
    action="store_true",
    help="Include the zeroth step in the timing averages",
)

parser.add_argument(
    "-o",
    "--by-operation",
    action="store_true",
    help="Plot times by operation, not by task subtype",
)
parser.add_argument(
    "-p",
    "--png",
    action="store_true",
    help="Make a .png image, not a pdf",
)
parser.add_argument(
    "-e",
    "--eagle",
    action="store_true",
    default=False,
    help="Plot outputs for Eagle runs",
)
parser.add_argument(
    "-g",
    "--gresho",
    action="store_true",
    default=True,
    help="Plot outputs for Gresho runs",
)
parser.add_argument(
    "-a",
    "--all-experiments",
    action="store_true",
    default=False,
    help="Plot outputs for all hardcoded experiment runs",
)
parser.add_argument(
    "--dine-grace",
    action="store_true",
    default=False,
    help="Plot results for dine2 and gracehopper",
)
parser.add_argument(
    "--dine-grace-gn001",
    action="store_true",
    default=False,
    help="Plot results for dine2 and gracehopper and gn001",
)
parser.add_argument(
    "--intel",
    action="store_true",
    default=False,
    help="Plot results for dine2 and gn001",
)


args = parser.parse_args()

# Which data do we include in the averages?
first_index = 1
if args.include_step_zero:
    first_index = 0

by_operation = args.by_operation
outfile_suffix = ""

if args.gresho:
    experiment_dirs = ["gresho256"]
    outfile_suffix = "_gresho"
if args.eagle:
    experiment_dirs = ["eagle12"]
    outfile_suffix = "_eagle12"
if args.all_experiments:
    experiment_dirs = ["gresho256", "eagle12"]
    outfile_suffix = "_all-experiments"

if args.dine_grace:
    node_dirs = ["dine2", "gn002"]
    outfile_suffix += "_dine-grace"

elif args.intel:
    node_dirs = ["dine2", "gn001"]
    outfile_suffix += "_dine-gn001"

elif args.dine_grace_gn001:
    node_dirs = ["dine2", "gn001", "gn002"]
    outfile_suffix += "_dine-grace-gn001"


# Read in data

results = {}

for node in node_dirs:

    results[node] = {}

    for experiment in experiment_dirs:

        fullfile = os.path.join(args.srcdir, node, experiment, timer_file)

        cols_to_use = []
        # add +1: 0th column in `timer_names` is "step"
        cols_to_use.append(timer_names.index("gpu_pack_density") + 1)
        cols_to_use.append(timer_names.index("gpu_pack_gradient") + 1)
        cols_to_use.append(timer_names.index("gpu_pack_force") + 1)
        cols_to_use.append(timer_names.index("gpu_unpack_density") + 1)
        cols_to_use.append(timer_names.index("gpu_unpack_gradient") + 1)
        cols_to_use.append(timer_names.index("gpu_unpack_force") + 1)
        cols_to_use.append(timer_names.index("gpu_launch_density") + 1)
        cols_to_use.append(timer_names.index("gpu_launch_gradient") + 1)
        cols_to_use.append(timer_names.index("gpu_launch_force") + 1)
        cols_to_use.append(timer_names.index("gpu_self_recurse") + 1)
        cols_to_use.append(timer_names.index("gpu_pair_recurse") + 1)

        data = np.loadtxt(fullfile, usecols=cols_to_use)

        density = {
            "pack": data[first_index:, 0].sum(),
            "unpack": data[first_index:, 3].sum(),
            "launch": data[first_index:, 6].sum(),
        }
        gradient = {
            "pack": data[first_index:, 1].sum(),
            "unpack": data[first_index:, 4].sum(),
            "launch": data[first_index:, 7].sum(),
        }
        force = {
            "pack": data[first_index:, 2].sum(),
            "unpack": data[first_index:, 5].sum(),
            "launch": data[first_index:, 8].sum(),
        }

        density["total"] = density["pack"] + density["unpack"] + density["launch"]
        gradient["total"] = gradient["pack"] + gradient["unpack"] + gradient["launch"]
        force["total"] = force["pack"] + force["unpack"] + force["launch"]

        experiment_result = {"density": density, "gradient": gradient, "force": force}

        results[node][experiment] = experiment_result


# Make plot

fig, ax = plt.subplots(1, 3, figsize=(10, 4), dpi=200, gridspec_kw={'width_ratios': [2, 2, 1]})
ax1 = ax[0]
ax2 = ax[1]
ax3 = ax[2]

# Calculate bar offset and positions
# How many bars are we going to have in total subtype?
nbars = len(node_dirs)

x = np.arange(3) # 3 for 3 task subtypes
width = 1.0 / (nbars + 1)

# bar/ticks are centered on bar at location of x, so it starts already
# shifted by width/2.
xticks = x + 0.5 * (nbars - 1) * width

hatches = [
    "\\\\\\\\\\\\",
    "----",
    "||||",
    "//////",
]



axes = [ax1, ax2]
for e, experiment in enumerate(experiment_dirs):
    ax = axes[e]

    for xind, task_type in enumerate(["density", "gradient", "force"]):
        for n, node in enumerate(node_dirs):

            pack = results[node][experiment][task_type]["pack"]
            unpack = results[node][experiment][task_type]["unpack"]
            launch = results[node][experiment][task_type]["launch"]
            total = results[node][experiment][task_type]["total"]

            hatch = hatches[n]
            pltkwargs = {
                #  "color": color,
                #  "edgecolor": color,
                "width": width,
                "hatch": hatch,
                "fill": False,
            }

            offset = n * width
            bottom = 0.

            ax.bar(x[xind] + offset, launch / total, **pltkwargs, bottom=bottom, color='C0', edgecolor='C0')
            bottom += launch / total

            ax.bar(x[xind] + offset, pack / total, **pltkwargs, bottom=bottom, color='C1', edgecolor='C1')
            bottom += pack / total

            ax.bar(x[xind] + offset, unpack / total, **pltkwargs, bottom=bottom, color='C2', edgecolor='C2')
            bottom += unpack / total

        ax.set_title(experiment_titles[experiment])
        ax.set_xticks(xticks, ["Density", "Gradient", "Force"])
        ax.set_ylim([0, 1.0])




# add 3rd ax just for legend


for n, node in enumerate(node_dirs):
    hatch = hatches[n]
    ax3.bar([0.], [1.], label=node_dir_labels[node], fill=False, color="k", edgecolor="k", hatch=hatch)
for t, task_type in enumerate(["Density", "Gradient", "Force"]):
    col = "C"+str(t)
    ax3.bar([0.], [1.], label=task_type, fill=True, color=col, edgecolor=col)

# move pseudo-plots outside visible region
ax3.set_xlim(2, 3)
ax3.set_ylim(2, 3)
ax3.legend(loc="center")
ax3.axis("off")


for ax in axes:
    #  ax.legend()
    ax.grid(axis="y")

#  ax1.set_ylabel("Fraction of time (per interaction loop)")
ax1.set_ylabel(r"$t_{\mathrm{op}}$ / $t_{\mathrm{tot}}$")

#  ax2.set_yticklabels([])
#  ax3.set_yticklabels([])


plt.tight_layout()  # rect=(0.05, 0.05, 0.95, 0.95))

#  plt.show()
figname = "gpu_timers_singlebar"
if by_operation:
    figname = "gpu_timers_by_operation_singlebar"
figname += outfile_suffix

if args.png:
    figname += ".png"
else:
    figname += ".pdf"

#  plt.show()
plt.savefig(figname)
print("saved", figname)
