# Load up a compiler configuration.
# Only load one of these, the one you need.
# include ../bin/config_local_HP_gcc.mk
# include ../bin/config_local_HP_icx.mk
include ../bin/config_cosma_gn001.mk

# Now load up global configuration. Always include this.
include ../bin/config.mk

# Note: order matters for linking. Most dependencies last.
OBJECTS = swiftgpupacksim.o io.o parameters.o help.o runner_gpu_pack_functions.o \
	  timers.o clocks.o task.o run_simulation.o 
CUDA_OBJECTS=gpu_data_buffers.o 

EXEC = swiftgpupacksim


default: $(EXEC)

$(EXEC): $(OBJECTS) $(CUDA_OBJECTS)
	$(NVCC) $(NVCC_FLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $(OBJECTS) $(CUDA_OBJECTS) -o $@

swiftgpupacksim.o: swiftgpupacksim.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

clocks.o: clocks.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

help.o: help.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

io.o: io.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

parameters.o: parameters.c 
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

run_simulation.o: run_simulation.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

runner_gpu_pack_functions.o: runner_gpu_pack_functions.c 
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

task.o: task.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<

timers.o: timers.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<



gpu_data_buffers.o: cuda/gpu_data_buffers.cu
	$(NVCC) $(NVCC_FLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -c $@ $<




# %.o: %.c
#         $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) -o $@ -c $<



clean:
	rm -f *.o $(EXEC)

debug:
	echo '$$(warning COMPILING IN DEBUG MODE)' > debug.mk

nodebug: 
	rm -f debug.mk

.PHONY: default all clean debug nodebug check

