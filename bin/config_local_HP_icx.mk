$(warning READING IN COMPILER SETTINGS FROM config_local_HP_icx.mk)


CC = icx

CFLAGS = -fno-lto -I//opt/cuda/include -O3 -ansi-alias -mavx512vbmi -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes
CFLAGS_DEBUG = -g -O0  -fno-lto -I//opt/cuda/include -debug inline-debug-info -O3 -ansi-alias -mavx512vbmi -pthread  -Wall -Wextra -Wshadow -Wstrict-prototypes

INCLUDES=
LD_LIBRARY_PATH=
LIBS=
SANITIZE_FLAGS=-fsanitize=address
DEFINES=-DCONFIG_LOCAL_HP_ICX

CFLAGS += -Wno-unused-parameter
CFLAGS_DEBUG += -Wno-unused-parameter
