# This file is part of SWIFT.
# Copyright (c) 2012 Pedro Gonnet (pedro.gonnet@durham.ac.uk),
#                    Matthieu Schaller (schaller@strw.leidenuniv.nl).
#                    mladen.ivkovic@durham.ac.uk
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Add the non-standard paths to the included library headers
AM_CFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/swift_placeholders -I$(top_srcdir)/src/swift_placeholders/cuda -DWITH_CUDA $(CFLAGS)
AM_LDFLAGS = $(LDFLAGS)

# Additional dependencies for shared libraries.
EXTRA_LIBS = $(NUMA_LIBS) $(PROFILER_LIBS) $(TCMALLOC_LIBS) $(JEMALLOC_LIBS) $(TBBMALLOC_LIBS) 

# Build the libswiftsim library and a convenience library just for the gravity tasks
lib_LTLIBRARIES = libswiftgpupacksim.la

# List required headers
include_HEADERS = swift_placeholders/cuda/gpu_offload_data.h
include_HEADERS += swift_placeholders/task.h
include_HEADERS += swift_placeholders/clocks.h
include_HEADERS += swift_placeholders/timers.h
include_HEADERS += swift_placeholders/gpu_pack_params.h
include_HEADERS += swift_placeholders/gpu_pack_metadata.h
include_HEADERS += swift_placeholders/engine.h
include_HEADERS += parameters.h
include_HEADERS += help.h
include_HEADERS += run_simulation.h
include_HEADERS += io.h

# Common source files
AM_SOURCES = swift_placeholders/cuda/gpu_offload_data.c
AM_SOURCES += swift_placeholders/task.c
AM_SOURCES += swift_placeholders/clocks.c
AM_SOURCES += swift_placeholders/timers.c
AM_SOURCES += swift_placeholders/gpu_pack_params.c
AM_SOURCES += swift_placeholders/gpu_pack_metadata.c
AM_SOURCES += swift_placeholders/engine.c
AM_SOURCES += parameters.c
AM_SOURCES += help.c
AM_SOURCES += run_simulation.c
AM_SOURCES += io.c

# Include files for distribution, not installation.
nobase_noinst_HEADERS = swift_placeholders/cuda/gpu_part_pack_functions.h
nobase_noinst_HEADERS += swift_placeholders/cuda/gpu_part_structs.h
nobase_noinst_HEADERS += swift_placeholders/minmax.h
nobase_noinst_HEADERS += swift_placeholders/barrier.h
nobase_noinst_HEADERS += swift_placeholders/atomic.h
nobase_noinst_HEADERS += swift_placeholders/cache.h
nobase_noinst_HEADERS += swift_placeholders/timestep_limiter_struct.h
nobase_noinst_HEADERS += swift_placeholders/scheduler.h
nobase_noinst_HEADERS += swift_placeholders/gravity_cache.h
nobase_noinst_HEADERS += swift_placeholders/active.h
nobase_noinst_HEADERS += swift_placeholders/align.h
nobase_noinst_HEADERS += swift_placeholders/swift_intrinsics.h
nobase_noinst_HEADERS += swift_placeholders/error.h
nobase_noinst_HEADERS += swift_placeholders/lock.h
nobase_noinst_HEADERS += swift_placeholders/cell_other_physics.h
nobase_noinst_HEADERS += swift_placeholders/runner.h
nobase_noinst_HEADERS += swift_placeholders/queue.h
nobase_noinst_HEADERS += swift_placeholders/rt_struct.h
nobase_noinst_HEADERS += swift_placeholders/cycle.h
nobase_noinst_HEADERS += swift_placeholders/hydro_part_placeholder_structs.h
nobase_noinst_HEADERS += swift_placeholders/threadpool.h
nobase_noinst_HEADERS += swift_placeholders/inline.h
nobase_noinst_HEADERS += swift_placeholders/timeline.h
nobase_noinst_HEADERS += parts.h
nobase_noinst_HEADERS += cell_hydro.h
nobase_noinst_HEADERS += hydro_part.h
nobase_noinst_HEADERS += cell.h
nobase_noinst_HEADERS += packing_data_struct.h
nobase_noinst_HEADERS += hydro_part_generated.h



# Sources and flags for regular library
libswiftgpupacksim_la_SOURCES = $(AM_SOURCES) $(CPU_SOURCES)
libswiftgpupacksim_la_CFLAGS = $(AM_CFLAGS) $(CUDA_FLAGS) $(MYFLAGS)
libswiftgpupacksim_la_LDFLAGS = $(AM_LDFLAGS) $(EXTRA_LIBS) $(CUDA_LDFLAGS) $(CUDA_LIBS)


