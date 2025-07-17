# Load up a compiler configuration.
include ../bin/Makefile


TEST_EXECS=test_cmdlineargs.o test_read_paramfile.o test_read_data_file.o

test: $(TEST_EXECS)

default: $(TEST_EXECS)

all: $(TEST_EXECS)


test_cmdlineargs.o: test_cmdlineargs.c io.o help.o parameters.o
	$(CC) $(CFLAGS) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $(SANITIZE_FLAGS) $< -o $@ io.o parameters.o help.o

test_read_data_file.o: test_read_data_file.c io.o parameters.o
	$(CC) $(CFLAGS) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $(SANITIZE_FLAGS) $< -o $@ io.o parameters.o help.o

test_read_paramfile.o: test_read_paramfile.c io.o parameters.o
	$(CC) $(CFLAGS) $(INCLUDES) $(LD_LIBRARY_PATH) $(LIBS) $(SANITIZE_FLAGS) $< -o $@ io.o parameters.o help.o


