$(warning READING IN COMPILER SETTINGS FROM config_local_HP_gcc.mk)


CC = gcc
# -Wno-unused-but-set-variable -Wno-unused-variable
CFLAGS = -flto=auto -fvar-tracking-assignments -O3 -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -march=znver2 -mavx2 -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes
CFLAGS_DEBUG = -g -O0 -O3 -flto=auto -gdwarf -fvar-tracking-assignments -fomit-frame-pointer -malign-double -fstrict-aliasing -ffast-math -funroll-loops -mavx2 -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes

INCLUDES=
LD_LIBRARY_PATH=
LIBS=
SANITIZE_FLAGS=-fsanitize=address

