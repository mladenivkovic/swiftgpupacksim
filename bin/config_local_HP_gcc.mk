$(warning READING IN COMPILER SETTINGS FROM config_local_HP_gcc.mk)


CC = gcc
NVCC = nvcc

CFLAGS = -fno-lto -I//opt/cuda/include -fvar-tracking-assignments -O3 -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=core2 -mavx512vbmi -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes
CFLAGS_DEBUG = -g -O0  -fno-lto -I//opt/cuda/include -gdwarf -fvar-tracking-assignments -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=core2 -mavx512vbmi -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes

NVCC_FLAGS = -O3 
NVCC_FLAGS_DEBUG = -O0 -g

INCLUDES=
LD_LIBRARY_PATH=-L/opt/cuda/lib
LIBS=-lcudart -lm
SANITIZE_FLAGS=-fsanitize=address
DEFINES=-DCONFIG_LOCAL_HP_GCC

