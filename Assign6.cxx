/***********************************************************
CSCI 480 - Assignment 6 - Fall 2019
 
Progammer: Sam Piecz
Z-ID: Z1732715
Section: 2 
TA: Jingwan Li  
Date Due: Nov 13, 2019 Purpose: Memory Management. 
************************************************************/
#include "Block.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <list>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <vector>
 
using std::istringstream;
using std::istream_iterator;
using std::ifstream;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::list;
 
#define HOW_OFTEN 5
 
// Global Vars
string filename = "data6.txt";
int inUseSize;
int availableSize;
int count = 0;
int kilobyte = 1024;
int megabyte = 1024*1024;
list<Block*> inUseBlocks = {};
list<Block*> availableBlocks = {
  new Block(3*1024*1024, megabyte, 0, ""), 
  new Block(4*1024*1024, megabyte*2, 0, ""),
  new Block(6*1024*1024, megabyte*2, 0, ""),
  new Block(8*1024*1024, megabyte*4, 0, ""),
  new Block(12*1024*1024, megabyte*4, 0, "")
};
 
// Prototypes
void load(int, int, string, string);
void allocate(int, int, string, string);
void deallocate(int, string);
void terminate(int);
void printStatus(bool transaction);
 
int main(int argc, char* argv[])
{
  // -1. If no arg handle error and exit(-1);
  if (argc < 2)
  {
    cout << "Must specify either a B or F." << endl;
    exit(-1);
  }
 
  // 0. Display Start Message.
  if (strcmp(argv[1], "B") == 0)
  {
    cout << "Simulation of Memory Management using the Best-Fit algorithm" << endl;
  }
  else if (strcmp(argv[1], "F") == 0)
  {
    cout << "Simulation of Memory Management using the First-Fit algorithm" << endl;
  }
 
  cout << "\nBeginning of the run" << endl;
 
  // Open and read input file. Process transactions.
  int id, size, processId; 
  string line, blockId;
  ifstream infile(filename);
  while (getline(infile, line))
  {
    // Ensure available blocks are sorted
    availableBlocks.sort();
 
    // Every HOW_OFTEN print inUseBlocks and availableBlocks.
    if (count % HOW_OFTEN == 0)
    {
      printStatus(false);
    }
 
    // Check for delimiter
    if (line.find("?") != string::npos)
    {
      break;
    }
 
    // Break line by spaces 
    istringstream iss(line);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
 
    // See what type of transaction and handle appropriately
    if (line.find("L") != string::npos)
    {
      id = stoi(tokens[1]); 
      size = stoi(tokens[2]); 
      blockId = tokens[3]; 
 
      if (strcmp(argv[1], "B") == 0)
      {
        load(id, size, blockId, "B");
      }
      else if (strcmp(argv[1], "F") == 0)
      {
        load(id, size, blockId, "F");
      }
 
    }
    else if (line.find("A") != string::npos)
    {
 
      processId = stoi(tokens[1]); 
      size = stoi(tokens[2]); 
      blockId = tokens[3];
 
      if (strcmp(argv[1], "B") == 0)
      {
        allocate(processId, size, blockId, "B");
      }
      else if (strcmp(argv[1], "F") == 0)
      {
        allocate(processId, size, blockId, "F");
      }
 
    }
    else if (line.find("D") != string::npos)
    {
      processId = stoi(tokens[1]);
      blockId = tokens[2];
      deallocate(processId, blockId);
    }
    else if (line.find("T") != string::npos)
    {
      processId = stoi(tokens[1]);
      terminate(processId);
    }
 
    count++;
 
  }
 
  // Display exit message. 
  cout << "\nEnd of the run" << endl;
 
  // Print empty lists.
  printStatus(false);
  return 0;
}
 
 
// Function Definitions
void load(int processId, int size, string blockId, string whichAlgorithm)
{
  int temp = 15 * megabyte;
  bool isSpace = false;
  Block* blockPtr;

  cout << "\nTransaction: request to load process " << processId << ", block ID " << blockId << " using " << size << " bytes" << endl; 
  if (whichAlgorithm == "B")
  {
    for (auto block : availableBlocks)
    {
      if ((block->size - size) < temp && (block->size - size) > 0)
      {
        temp = (block->size - size);
        blockPtr = block;
        isSpace = true;
      }
    }
    if (isSpace)
    {
      for (auto block : availableBlocks)
      {
        if (block->startingAddress == blockPtr->startingAddress)
        {
          block->size -= size;
          startingAddress = block->startingAddress;
          block->startingAddress += size;

          inUseBlocks.push_front(new Block(startingAddress, size, id, name));

          cout << "Found a block of size " << blockPtr->size << endl;
          cout << "Success in allocating a block" << endl;
          
          break;
        }
      }
    }
    else
    {
      cout << "Unable to load or allocate: " << id << endl;
    }
  }
  else if (whichAlgorithm == "F")
  {

    for (auto block : availableBlocks)
    {
      if ((block->size - size) >= 0)
      {
        isSpace = true;
        blockPtr = block;
        break;
      }
    }

    if (isSpace)
    {
      blockPtr->size -= size;
      startingAddress = blockPtr->startingAddress;
      blockPtr->startingAddress += size;
      inUseBlocks.push_front(new Block(startingAddress, size, id, name));

      cout << "Found a block of size " << blockPtr->size << endl
      << "Success in allocating a block" << endl;
      break;
    }
    else
    { 
      cout << "Unable to load or allocate: " << id << endl;
    }
  }
  printStatus(false);
}
 
void allocate(int processId, int size, string blockId, string whichAlgorithm)
{
  int temp = 15 * megabyte;
  bool isSpace = false;
  Block* blockPtr;

  cout << "\nTransaction: request to load process " << processId << ", block ID " << blockId << " using " << size << " bytes" << endl; 
  if (whichAlgorithm == "B")
  {
    for (auto block : availableBlocks)
    {
      if ((block->size - size) < temp && (block->size - size) > 0)
      {
        temp = (block->size - size);
        blockPtr = block;
        isSpace = true;
      }
    }
    if (isSpace)
    {
      for (auto block : availableBlocks)
      {
        if (block->startingAddress == blockPtr->startingAddress)
        {
          block->size -= size;
          startingAddress = block->startingAddress;
          block->startingAddress += size;

          inUseBlocks.push_front(new Block(startingAddress, size, id, name));

          cout << "Found a block of size " << blockPtr1->size << endl;
          cout << "Success in allocating a block" << endl;
          
          break;
        }
      }
    }
    else
    {
      cout << "Unable to load or allocate: " << id << endl;
    }
  }
  else if (whichAlgorithm == "F")
  {

    for (auto block : availableBlocks)
    {
      if ((block->size - size) >= 0)
      {
        isSpace = true;
        blockPtr = block;
        break;
      }
    }

    if (isSpace)
    {
      blockPtr->size -= size;
      startingAddress = blockPtr->startingAddress;
      blockPtr->startingAddress += size;
      inUseBlocks.push_front(new Block(startingAddress, size, id, name));

      cout << "Found a block of size " << blockPtr->size << endl
      << "Success in allocating a block" << endl;
      break;
    }
    else
    { 
      cout << "Unable to load or allocate: " << id << endl;
    }
  }
  printStatus(false);
}
 
void deallocate(int processId, string blockId)
{
  cout << "Transaction: request to deallocate process " << id << endl;
  Block* tempIter;
  bool found = false;

  for (auto block : inUseBlocks)
  { 
    if (block->ID == id)
    {
      Block* theBlock = new Block(block->startingAddress, block->size, 0, "");
      
      /*
      for (auto blockPtr : availableBlocks)
      {
        if (blockPtr->startingAddress > block->startingAddress)
        {
          availableBlocks.insert(blockPtr, theBlock);
          found = true;
          break;
        }
      }
      */

      if (!found)
      {
        availableBlocks.push_back(theBlock);
      }

      /*
      for (auto blockPtr : availableBlocks)
      {
        tempIter = blockPtr;
        tempIter++;
        if ((blockPtr->size + blockPtr->startingAddress) == (tempIter->startingAddress))
        {
          if ((blockPtr->size + tempIter->size) <= 4 * megabyte)
          {
            cout << "Merging two blocks at " << blockPtr->startingAddress << " and " << tempIter->startingAddress << endl;
            blockPtr->size = blockPtr->size + tempIter->size;
            availableBlocks.erase(tempIter);
            blockPtr--;
          }
        }
      }
      */

      delete block;
      found = true;
    }
  }

  if (!found)
  {
    cout << "Unable to terminate id: " << id << endl;
  }
  else
  { 
    cout << "Success in terminating a process" << endl;
  }

}
 
void terminate(int processId)
{
  cout << "Transaction: request to terminate process " << id << endl;
  Block* tempIter;
  bool found = false;

  for (auto block : inUseBlocks)
  { 
    if (block->ID == id)
    {
      Block* theBlock = new Block(block->startingAddress, block->size, 0, "");

      for (auto blockPtr : availableBlocks)
      {
        if (blockPtr->startingAddress > block->startingAddress)
        {
          availableBlocks.insert(blockPtr, theBlock);
          found = true;
          break;
        }
      }
      if (!found)
      {
        availableBlocks.push_back(theBlock);
      }

      for (auto blockPtr : availableBlocks)
      {
        tempIter = blockPtr;
        tempIter++;
        if ((blockPtr->size + blockPtr->startingAddress) == (tempIter->startingAddress))
        {
          if ((blockPtr->size + tempIter->size) <= 4 * megabyte)
          {
            cout << "Merging two blocks at " << blockPtr->startingAddress << " and " << tempIter->startingAddress << endl;
            blockPtr->size = blockPtr->size + tempIter->size;
            availableBlocks.erase(tempIter);
            blockPtr--;
          }
        }
      }
      delete block;
      found = true;
    }
  }

  if (!found)
  {
    cout << "Unable to terminate id: " << id << endl;
  }
  else
  { 
    cout << "Success in terminating a process" << endl;
  }

}
 
void printStatus(bool transaction)
{
  // Print available blocks & size
  cout << "\nList of available blocks" << endl;
  for (auto block : availableBlocks)
  {
    block->print(transaction);
    availableSize += block->size;
  }
  cout << "Total size of the list = " << availableSize << endl; 
 
 
  // Print in use blocks & size
  cout << "\nList of blocks in use" << endl;
  if (inUseBlocks.empty())
  {
    cout << " (none)" << endl;
  }
  else
  {
    for (auto block : inUseBlocks)
    {
      block->print(true);
      inUseSize += block->size;
    }
  }
  cout << "Total size of the list = " << inUseSize << endl; 
 
  // Reset counters
  inUseSize = 0;
  availableSize = 0;
}
