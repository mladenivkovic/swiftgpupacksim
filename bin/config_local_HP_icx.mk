$(warning READING IN COMPILER SETTINGS FROM config_local_HP_icx.mk)


CC = icx

CFLAGS = -fno-lto -ipo -O3 -ansi-alias -mavx512vbmi -pthread -Wall -Wextra -Wno-unused-parameter -Wshadow -Werror -Wstrict-prototypes -Wno-unused-command-line-argument
CFLAGS_DEBUG = -g -O0  -fno-lto -debug inline-debug-info -O3 -ansi-alias -mavx512vbmi -pthread -Wall -Wextra -Wno-unused-parameter -Wshadow -Werror -Wstrict-prototypes -Wno-unused-command-line-argument

INCLUDES=
LD_LIBRARY_PATH=
# LD_LIBRARY_PATH=-L/opt/cuda/lib
LIBS=-pthread -lm
SANITIZE_FLAGS=-fsanitize=address
DEFINES=-DCONFIG_LOCAL_HP_ICX -DWITH_CUDA

