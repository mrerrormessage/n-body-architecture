COMPILER=gcc
FILE=serial.c
FLAGS=-Wall -g -std=c99 -lm
TESTFILE=unittest.c
MAKENAME=serial
LINKFILES=cosmos.o nbodyutil.o

serial.exe : $(LINKFILES)
	$(COMPILER) $(FLAGS) $(FILE) $(LINKFILES) -o $(MAKENAME)

cosmos.o : nbodyutil.o
	$(COMPILER) $(FLAGS) nbodyutil.o -c cosmos.c -o cosmos.o

nbodyutil.o:
	$(COMPILER) $(FLAGS) -c nbodyutil.c -o nbodyutil.o

test : 
	$(COMPILER) $(FLAGS) $(TESTFILE) $(FILE) -o testrun

clean : 
	rm -f core *~ *.o *.a $(MAKENAME)
