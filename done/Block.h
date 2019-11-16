/***********************************************************
 CSCI 480 - Assignment 6 - Fall 2019
 
 Progammer: Sam Piecz
 Z-ID: Z1732715
 Section: 2 
 TA: Jingwan Li  
 Date Due: Nov 13, 2019 
 Purpose: Memory Management. 
 ************************************************************/
#ifndef BLOCK_H
#define BLOCK_H

#include <string>
using std::string;

class Block {
  public:
    Block(int, int, int, string);
    void print(bool transaction);
    int size;
    bool operator<(const Block &rhs) const { return size < rhs.size; }
    int startingAddress;
    int processId;
    string blockId;
};

#endif
