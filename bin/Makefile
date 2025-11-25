# Load up a compiler configuration.
# Only load one of these, the one you need.
# include ../bin/config_local_HP_gcc.mk
include ../bin/config_local_HP_icx.mk
# include ../bin/config_cosma_gn001.mk

# Now load up global configuration. Always include this.
include ../bin/config.mk

# Note: order matters for linking. Most dependencies last.
OBJECTS = swiftgpupacksim.o parameters.o help.o run_simulation.o io.o \
	  gpu_offload_data.o \
	  task.o clocks.o timers.o gpu_pack_params.o gpu_pack_metadata.o engine.o 
CUDA_OBJECTS=

EXEC = swiftgpupacksim


default: $(EXEC)

$(EXEC): $(OBJECTS) $(CUDA_OBJECTS)
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $(OBJECTS) $(CUDA_OBJECTS) -o $@

swiftgpupacksim.o: swiftgpupacksim.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

parameters.o: parameters.c 
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

help.o: help.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

run_simulation.o: run_simulation.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

io.o: io.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

gpu_offload_data.o: swift_placeholders/cuda/gpu_offload_data.c
	echo CALLED THIS
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

task.o: swift_placeholders/task.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

clocks.o: swift_placeholders/clocks.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

timers.o: swift_placeholders/timers.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

gpu_pack_params.o: swift_placeholders/gpu_pack_params.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

gpu_pack_metadata.o: swift_placeholders/gpu_pack_metadata.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

engine.o: swift_placeholders/engine.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

#         $(NVCC) $(NVCC_FLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -c $@ $<




# %.o: %.c
#         $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<



clean:
	rm -f *.o $(EXEC)

debug:
	echo '$$(warning COMPILING IN DEBUG MODE)' > debug.mk

nodebug: 
	rm -f debug.mk

.PHONY: default all clean debug nodebug check

