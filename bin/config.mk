# Make sure you load this only after the compiler settings!

$(warning reading in global settings)

SRCDIR=../src

#include paths. Will be followed in that order.
VPATH=$(SRCDIR)

#include directories for headers
IDIR=$(SRCDIR)


VPATH=../src:../src/cuda

INCLUDES+=-I. -I$(SRCDIR)



ifneq ("$(wildcard debug.mk)","")
include debug.mk
CFLAGS=$(CFLAGS_DEBUG) -DSWIFT_DEBUG_CHECKS
endif


