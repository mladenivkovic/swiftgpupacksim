# This file is part of SWIFT.
# Copyright (c) 2012 pedro.gonnet@durham.ac.uk
#               2015 schaller@strw.leidenuniv.nl
#               2025 mladen.ivkovic@durham.ac.uk
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

# Automake stuff
ACLOCAL_AMFLAGS = -I m4

# Show the way...
SUBDIRS = src tests 

# Common flags
MYFLAGS = -DWITH_CUDA

# Add the source directory and the non-standard paths to the included library headers to CFLAGS
AM_CFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/swift_placeholders -I$(top_srcdir)/src/swift_placeholders/cuda

AM_LDFLAGS = 

# Extra libraries.
EXTRA_LIBS = $(NUMA_LIBS) $(PROFILER_LIBS) $(TCMALLOC_LIBS) $(JEMALLOC_LIBS) $(TBBMALLOC_LIBS) 

# Programs.
bin_PROGRAMS = swiftgpupack

# Sources for swift
swiftgpupack_SOURCES = swiftgpupacksim.c
swiftgpupack_CFLAGS = $(AM_CFLAGS) $(CUDA_FLAGS) $(MYFLAGS)
swiftgpupack_LDADD =  src/libswiftgpupacksim.la $(AM_LDFLAGS) $(EXTRA_LIBS) $(CUDA_LDFLAGS) $(CUDA_LIBS)

# Non-standard files that should be part of the distribution.
EXTRA_DIST = 
