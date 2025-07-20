$(warning READING IN COMPILER SETTINGS FROM config_cosma_gn001.mk)

CC = gcc
NVCC = nvcc

CFLAGS = -flto=auto -flto -O3 -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=skylake-avx512 -mavx512dq -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes
CFLAGS_DEBUG = -g -flto=auto -flto -O3 -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=skylake-avx512 -mavx512dq -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes

INCLUDES=
LD_LIBRARY_PATH=
LIBS=
SANITIZE_FLAGS=-fsanitize=address
DEFINES=-DCONFIG_COSMA_GN001_GCC

NVCC_FLAGS = -D_FORCE_INLINES -O4 -lineinfo -src-in-ptx --maxrregcount=64 -ftz=true -DWITH_CUDA --default-stream per-thread --use_fast_math -lcudadevrt #-dlink -ccbin=gcc 
NVCC_FLAGS += -arch=sm_70
NVCC_FLAGS += --extra-device-vectorization

NVCC_FLAGS_DEBUG = -g -G -D_FORCE_INLINES -O4 -src-in-ptx --maxrregcount=64 -ftz=true -DWITH_CUDA --default-stream per-thread --use_fast_math -lcudadevrt #-dlink -ccbin=gcc 
NVCC_FLAGS_DEBUG += -arch=sm_70
NVCC_FLAGS_DEBUG += --extra-device-vectorization


