#!/usr/bin/env python3

"""
Lists of various required data used in internal consistency checks.
"""


_required_particle_file_metadata_swift = [
    "authors",
    "flavour",
]

_required_other_file_metadata_swift = [
    "authors",
]

# basically everything in the "None" flavour
_required_particle_fields = [
    "id",
    "gpart",
    "x",
    "v",
    "a_hydro",
    "mass",
    "h",
    "rho",
    "wcount",
    "wcount_dh",
    "h_dt",
    "adaptive_softening_data",
    "mhd_data",
    "chemistry_data",
    "cooling_data",
    "feedback_data",
    "black_holes_data",
    "sink_data",
    "pressure_floor_data",
    "rt_data",
    "rt_time_data",
    "depth_h",
    "time_bin",
    "timestep_limiter",
    "ti_drift",
    "ti_kick",
    "geometry",
    "x_diff",
    "x_diff_sort",
    "v_full",
    "a_grav",
    "extra_split_data",
    "extra_cooling_data",
    "extra_tracers_data",
    "extra_sf_data",
    "extra_feedback_data",
    "extra_mhd_data",
    "extra_forcing_data",
    "csds_data",
]
