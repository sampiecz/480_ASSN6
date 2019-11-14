# PROGRAM:    Assign 6 
# PROGRAMMER: Samuel Piecz
# LOGON ID:   Z1732715
# DATE DUE:   11/13/19
#

CCFLAGS = -ggdb -Wall -std=c++11

Assign6: Assign6.o Block.o
	g++ $(CCFLAGS) -o Assign6 Assign6.o Block.o

Assign6.o: Assign6.cxx  Block.h 
	g++ $(CCFLAGS) -c Assign6.cxx

Block.o: Block.cxx Block.h
	g++ $(CCFLAGS) -c Block.cxx

clean:
	-rm *.o Assign6
