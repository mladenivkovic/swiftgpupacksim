# This file is part of SWIFT.
# Copyright (c) 2015 schaller@strw.leidenuniv.nl.
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

# Add the source directory and the non-standard paths to the included library headers to CFLAGS
AM_CFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/swift_placeholders -I$(top_srcdir)/src/swift_placeholders/cuda $(NUMA_INCS) 

AM_LDFLAGS = ../src/.libs/libswiftgpupacksim.a $(NUMA_LIBS) $(TCMALLOC_LIBS) $(JEMALLOC_LIBS) $(TBBMALLOC_LIBS) 

# List of programs and scripts to run in the test suite
TESTS = test_cmdlineargs test_header_output test_read_data_file test_read_paramfile pytests.sh

# List of test programs to compile
check_PROGRAMS = test_cmdlineargs test_header_output test_read_data_file test_read_paramfile

# Rebuild tests when SWIFT is updated.
$(check_PROGRAMS): ../src/.libs/libswiftgpupacksim.a

# Sources for the individual programs
test_cmdlineargs_SOURCES = test_cmdlineargs.c

test_header_output_SOURCES = test_header_output.c

test_read_data_file_SOURCES = test_read_data_file.c

test_read_paramfile_SOURCES = test_read_paramfile.c

# Files necessary for distribution
EXTRA_DIST = input/example_measurement.dat input/log_runtime_params.dat input/test_arrays.yml \
	input/test_data_types.yml input/test_ifdefs.yml input/test_split_struct_arrays.yml \
	input/test_split_struct_nopart.yml input/test_split_struct_structs_and_unions.yml \
	input/test_split_struct.yml input/test_struct.yml input/test_union.yml \
	test_arrays.h test_data_types.h test_ifdefs.h test_split_struct_arrays.h test_split_struct.h \
	test_split_struct_nopart.h test_split_struct_nopart.yml test_split_struct_structs_and_unions.h \
	test_struct.h test_union.h
