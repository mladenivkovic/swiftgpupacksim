# Load up a compiler configuration.
include ../bin/config_local_HP_gcc.mk

# load up global configuration
include ../bin/config.mk

# Note: order matters for linking. Most dependencies last.
OBJECTS = io.o parameters.o help.o 
EXEC = swiftgpupack


default: $(EXEC)

swiftgpupack: swiftgpupack.c $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $< -o $@ $(OBJECTS)


help.o: help.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS)

io.o: io.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS)

parameters.o: parameters.c 
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS)



clean:
	rm -f *.o $(EXEC)

debug:
	echo '$$(warning COMPILING IN DEBUG MODE)' > debug.mk

nodebug: 
	rm -f debug.mk

.PHONY: default all clean debug nodebug check

