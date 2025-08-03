# Make sure you load this only after the compiler settings!

$(warning reading in global settings)

SRCDIR=../src

#include paths. Will be followed in that order.
VPATH=$(SRCDIR)

#include directories for headers
IDIR=$(SRCDIR)


VPATH=$(SRCDIR):$(SRCDIR)/cuda:$(SRCDIR)/swift_placeholders

INCLUDES+=-I. -I$(SRCDIR) -I$(SRCDIR)/cuda -I$(SRCDIR)/swift_placeholders

$(warning dev mode -  adding -Wno-unused-parameter)
# CFLAGS += -Wno-unused-parameter
# CFLAGS_DEBUG += -Wno-unused-parameter

ifneq ("$(wildcard debug.mk)","")
include debug.mk
CFLAGS=$(CFLAGS_DEBUG) -DSWIFT_DEBUG_CHECKS
NVCC_FLAGS=$(NVCC_FLAGS_DEBUG) -DSWIFT_DEBUG_CHECKS
endif


