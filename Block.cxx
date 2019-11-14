/***********************************************************
 CSCI 480 - Assignment 5 - Fall 2019
 
 Progammer: Sam Piecz
 Z-ID: Z1732715
 Section: 2 
 TA: Jingwan Li  
 Date Due: Oct 25, 2019 
 Purpose: Memory management. 
 ************************************************************/
#include "Block.h"
#include <iostream>

using std::cout;
using std::endl;

Block::Block(int startingAddress, int size, int ownerProcessId, string blockId)
{
  this->startingAddress = startingAddress;
  this->size = size;
  this->ownerProcessId = ownerProcessId;
  this->blockId = blockId;
}

void Block::print()
{
  cout << "Start Address = " << this->startingAddress << " Size = " << this->size << endl;
}
