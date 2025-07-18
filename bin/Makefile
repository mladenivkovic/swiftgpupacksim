# Load up a compiler configuration.
include ../bin/config_local_HP_gcc.mk
# include ../bin/config_cosma_gn001.mk

# load up global configuration
include ../bin/config.mk

# Note: order matters for linking. Most dependencies last.
OBJECTS = io.o parameters.o help.o timers.o clocks.o task.o
EXEC = swiftgpupacksim


default: $(EXEC)

swiftgpupacksim: swiftgpupack.c $(OBJECTS) parts.h
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $< -o $@ $(OBJECTS)


help.o: help.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

io.o: io.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

parameters.o: parameters.c 
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<



clean:
	rm -f *.o $(EXEC)

debug:
	echo '$$(warning COMPILING IN DEBUG MODE)' > debug.mk

nodebug: 
	rm -f debug.mk

.PHONY: default all clean debug nodebug check

