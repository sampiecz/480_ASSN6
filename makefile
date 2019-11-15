# PROGRAM:    Assign 6 
# PROGRAMMER: Samuel Piecz
# LOGON ID:   Z1732715
# DATE DUE:   11/13/19
#

CCFLAGS = -ggdb -Wall -std=c++11

Assign6: Assign6B.o Block.o
	g++ $(CCFLAGS) -o Assign6 Assign6B.o Block.o

Assign6B.o: Assign6B.cxx  Block.h 
	g++ $(CCFLAGS) -c Assign6B.cxx

Block.o: Block.cxx Block.h
	g++ $(CCFLAGS) -c Block.cxx

clean:
	-rm *.o Assign6
