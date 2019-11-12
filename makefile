# PROGRAM:    Assign 6 
# PROGRAMMER: Samuel Piecz
# LOGON ID:   Z1732715
# DATE DUE:   11/13/19
#

CCFLAGS = -ggdb -Wall -std=c++11

Assign6: Assign6.o Memory.o
	g++ $(CCFLAGS) -o Assign6 Assign6.o Memory.o

Assign6.o: Assign6.cxx  Memory.h 
	g++ $(CCFLAGS) -c Assign6.cxx

Memory.o: Memory.cxx Memory.h
	g++ $(CCFLAGS) -c Memory.cxx

clean:
	-rm *.o Assign6
