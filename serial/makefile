COMPILER=gcc
FILE=serial.c
FLAGS=-Wall -g -std=c99 -lm
TESTFILE=unittest.c
MAKENAME=serial

serial.exe : 
	$(COMPILER) $(FLAGS) $(FILE) -o $(MAKENAME)

test : 
	$(COMPILER) $(FLAGS) $(TESTFILE) $(FILE) -o testrun

clean : 
	rm -f core *~ *.o *.a $(MAKENAME)