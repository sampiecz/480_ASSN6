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

class Block {
  public:
    void print();
  private:
    int startingAddress;
    int size;
    string ownerProcessId;
    string blockId;
    Block* blockPtr1;
    Block* blockPtr2;
};

#endif
