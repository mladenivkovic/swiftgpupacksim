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
#  experiment_dirs = ["eagle12", "gresho256"]
experiment_dirs = ["gresho256"]
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
    "xtick.labelsize": 14,
    "ytick.labelsize": 14,
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
    help="Plot results for dine2 and gracehopper"
)
parser.add_argument(
    "--dine-grace-gn001",
    action="store_true",
    default=False,
    help="Plot results for dine2 and gracehopper and gn001"
)
parser.add_argument(
    "--intel",
    action="store_true",
    default=False,
    help="Plot results for dine2 and gn001"
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

fig = plt.figure(figsize=(14, 4), dpi=200)
ax1 = fig.add_subplot(131)
ax2 = fig.add_subplot(132, sharey=ax1)
ax3 = fig.add_subplot(133, sharey=ax1)


nbars = len(node_dirs) + len(experiment_dirs)

x = np.arange(3)
width = 1.0 / (nbars + 1)

# bar/ticks are centered on bar at location of x, so it starts already
# shifted by width/2.
xticks = x + 0.5 * (nbars - 1) * width

hatches = [
    "\\\\\\\\\\\\",
    "//////",
    "||",
    "--",
]


def plot_by_task_subtype(ax, task_type, title):
    """
    Plots the data of a single task (sub)type onto an axis.

    Parameters
    ----------

    ax: axis object to plot onto
    task_type: task type (density, force, gradient) to plot
    title: title to add to the axis
    """

    index = 0
    for n, node in enumerate(node_dirs):
        for e, experiment in enumerate(experiment_dirs):

            pack = results[node][experiment][task_type]["pack"]
            unpack = results[node][experiment][task_type]["unpack"]
            launch = results[node][experiment][task_type]["launch"]
            total = results[node][experiment][task_type]["total"]

            color = "C" + str(n)
            offset = index * width
            #  label = node + "/" + experiment
            label = node_dir_labels[node]
            hatch = hatches[e]
            pltkwargs = {
                "color": color,
                "edgecolor": color,
                "width": width,
                "hatch": hatch,
                "fill": False,
            }

            ax.bar(x[0] + offset, pack / total, **pltkwargs, label=label)
            ax.bar(x[1] + offset, launch / total, **pltkwargs)
            ax.bar(x[2] + offset, unpack / total, **pltkwargs)

            print(node, "pack:", pack/total * 100, "%")
            print(node, "launch:", launch/total * 100, "%")
            print(node, "unpack:", unpack/total * 100, "%")

            index += 1

    ax.set_title(title)
    ax.set_xticks(xticks, ["pack", "launch", "unpack"])

    return


def plot_by_operation(ax, operation, title):
    """
    Plots the data of a single operation onto an axis.

    Parameters
    ----------

    ax: axis object to plot onto
    operation: string of operation name (pack, unpack, launch) for packing
    timer
    title: title to add to the axis
    """

    index = 0
    for n, node in enumerate(node_dirs):
        for e, experiment in enumerate(experiment_dirs):

            density = results[node][experiment]["density"][operation]
            gradient = results[node][experiment]["gradient"][operation]
            force = results[node][experiment]["force"][operation]

            total_density = results[node][experiment]["density"]["total"]
            total_gradient = results[node][experiment]["gradient"]["total"]
            total_force = results[node][experiment]["force"]["total"]

            color = "C" + str(n)
            offset = index * width
            #  label = node + "/" + experiment
            label = node_dir_labels[node]
            if len(experiment_dirs) > 1:
                label = experiment + " " + label
            hatch = hatches[e]
            pltkwargs = {
                "color": color,
                "edgecolor": color,
                "width": width,
                "hatch": hatch,
                "fill": False,
            }

            ax.bar(x[0] + offset, density / total_density, **pltkwargs, label=label)
            ax.bar(x[1] + offset, gradient / total_gradient, **pltkwargs)
            ax.bar(x[2] + offset, force / total_force, **pltkwargs)

            print(experiment, node, operation, "density:", density/total_density * 100, "%")
            print(experiment, node, operation, "gradient:", gradient/total_gradient * 100, "%")
            print(experiment, node, operation, "force:", force/total_force * 100, "%")
            index += 1

    ax.set_title(title)
    ax.set_xticks(xticks, ["Density", "Gradient", "Force"])
    ax.set_ylim([0, 1.0])

    return


if by_operation:
    plot_by_operation(ax1, "pack", "Packing")
    plot_by_operation(ax2, "launch", "Launch")
    plot_by_operation(ax3, "unpack", "Unpacking")


else:
    plot_by_task_subtype(ax1, "density", "Density")
    plot_by_task_subtype(ax2, "gradient", "Gradient")
    plot_by_task_subtype(ax3, "force", "Force")


for ax in fig.axes:
    #  ax.legend()
    ax.grid(axis="y")

#  ax1.set_ylabel("Fraction of total time (per interaction loop)")
ax1.set_ylabel(r"$t_{\mathrm{op}}$ / $t_{\mathrm{tot}}$")
ax3.legend()
#  ax2.set_yticklabels([])
#  ax3.set_yticklabels([])


plt.tight_layout()  # rect=(0.05, 0.05, 0.95, 0.95))

#  plt.show()
figname = "gpu_timers"
if by_operation:
    figname = "gpu_timers_by_operation"
figname += outfile_suffix

if args.png:
    figname += ".png"
else:
    figname += ".pdf"

#  plt.show()
plt.savefig(figname)
print("saved", figname)
