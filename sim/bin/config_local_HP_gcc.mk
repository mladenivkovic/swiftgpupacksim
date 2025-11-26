$(warning READING IN COMPILER SETTINGS FROM config_local_HP_gcc.mk)


CC = gcc

CFLAGS = -fno-lto -I//opt/cuda/include -fvar-tracking-assignments -O3 -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=core2 -mavx512vbmi -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes
CFLAGS_DEBUG = -g -O0  -fno-lto -I//opt/cuda/include -gdwarf -fvar-tracking-assignments -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=core2 -mavx512vbmi -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes

INCLUDES=
LD_LIBRARY_PATH=-L/opt/cuda/lib
LIBS=-pthread -lcudart -lm
SANITIZE_FLAGS=-fsanitize=address
DEFINES=-DCONFIG_LOCAL_HP_GCC -DWITH_CUDA

