CUDACOMPILER=nvcc
COMPILER=gcc
FILE=cudanbody.cu
FLAGS=-Wall -g -std=c99 -lm
CUDAFLAGS=-deviceemu
TESTFILE=cudahello.cu
TESTNAME=cudahello
MAKENAME=cudanbody
LINKFILES=cosmos1.o nbodyutil1.o
CUDALINKFILES=cosmos.o nbdyutil2.o
CUDALINKFLAGS=--host-compilation c --compiler-options "-std=c99"

simple: 
	$(CUDACOMPILER) $(CUDAFLAGS) cudanbody.cu -o $(MAKENAME)

cosmos: 
	$(CUDACOMPILER) -lib $(CUDAFLAGS) cosmos.cu -o cosmos.lib

hello: 
	$(CUDACOMPILER) $(CUDAFLAGS) $(TESTFILE) -o $(TESTNAME)

test : 
	$(COMPILER) $(FLAGS) $(TESTFILE) $(FILE) -o testrun

clean : 
	rm -f core *~ *.o *.a $(MAKENAME)