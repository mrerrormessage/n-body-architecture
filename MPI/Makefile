COMPILER=mpicc
FILE=mpinbody.c
LIBS=../serial/serial.c
FLAGS=-Wall -g -std=c99 -lm
TESTFILE=unittest.c
MAKENAME=mpinbody
LINKFILES=cosmos.o nbodyutil.o

mpi.exe : $(LINKFILES)
	$(COMPILER) $(FLAGS) $(FILE) $(LIBS) -o $(MAKENAME)

cosmos.o : nbodyutil.o
	$(COMPILER) $(FLAGS) nbodyutil.o -c ../lib/cosmos.c -o cosmos.o

nbodyutil.o:
	$(COMPILER) $(FLAGS) -c ../lib/nbodyutil.c -o nbodyutil.o

test : 
	$(COMPILER) $(FLAGS) $(TESTFILE) $(FILE) -o testrun

clean : 
	rm -f core *~ *.o *.a $(MAKENAME)
