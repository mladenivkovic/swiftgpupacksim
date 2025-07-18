$(warning READING IN COMPILER SETTINGS FROM config_cosma_gn001.mk)

CC = gcc
CFLAGS = -flto=auto -flto -O3 -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=skylake-avx512 -mavx512dq -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes
CFLAGS_DEBUG = -g $(CFLAGS)

INCLUDES=
LD_LIBRARY_PATH=
LIBS=
SANITIZE_FLAGS=-fsanitize=address
DEFINES=-DCONFIG_COSMA_GN001_GCC

