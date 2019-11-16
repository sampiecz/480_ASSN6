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
int startingAddress = 3 * megabyte;
list<Block> inUseBlocks = {};
list<Block> availableBlocks = {
  Block(3*1024*1024, megabyte, 0, ""), 
  Block(4*1024*1024, megabyte*2, 0, ""),
  Block(6*1024*1024, megabyte*2, 0, ""),
  Block(8*1024*1024, megabyte*4, 0, ""),
  Block(12*1024*1024, megabyte*4, 0, "")
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
 
  // 0.5 
  cout << "\nBeginning of the run" << endl;
 
  // 2. Open and read input file. Process transactions.
  int id, size, processId; 
  string line, blockId;
  ifstream infile(filename);
  while (getline(infile, line))
  {
    // Ensure available blocks are sorted
    availableBlocks.sort();
 
    // 3. Every HOW_OFTEN print inUseBlocks and availableBlocks.
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
 
  // 5. Close file.
  // Never opened one so not necesssary.
 
  // 6. Display exit message. 
    cout << "\nEnd of the run" << endl;
 
  // 4. Print empty lists.
  printStatus(false);
  return 0;
}
 
 
// Function Definitions
void load(int processId, int size, string blockId, string whichAlgorithm)
{
  list<Block>::iterator iteratorA, iteratorB;
  int temporarySize = 15 * megabyte; 
  bool found = false, other = false;
  cout << "\nTransaction: request to load process " << processId << ", block ID " << blockId << " using " << size << " bytes" << endl; 
  if (whichAlgorithm == "B")
  {
    for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++) 
    {
      if((iteratorA->size - size) < temporarySize && (iteratorA->size - size) > 0)
      {
        temporarySize = (iteratorA->size - size);
        iteratorB = iteratorA;
        found = true;
        other = true;
      }
    }
    if (other == true)
    {
      for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++)
      {
        if (iteratorA->startingAddress == iteratorB->startingAddress)
        {
          iteratorA->size -= size;
          startingAddress = iteratorA->startingAddress;
          iteratorA->startingAddress += size;
          inUseBlocks.push_front(Block(startingAddress, size, processId, blockId)); 
          break;
        }
      }
    }
  }
  else if (whichAlgorithm == "F")
  {
    for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++)
    {
      if ((iteratorA->size - size) >= 0)
      {
        cout << "Found a block of size " << iteratorA->size << endl;
        iteratorA->size -= size;
        startingAddress = iteratorA->startingAddress;
        iteratorA->startingAddress += size;
        inUseBlocks.push_front(Block(iteratorA->startingAddress, size, processId, blockId));
        found = true;
        break;
      }
    }
  }
  if (found == false)
  {
    cout << "There is no sufficiently large block." << endl;
  }
  else
  {
    cout << "Success in allocating a block" << endl;
  }
  printStatus(false);
}
 
void allocate(int processId, int size, string blockId, string whichAlgorithm)
{
  list<Block>::iterator iteratorA, iteratorB;
  int temporarySize = 15 * megabyte;
  bool found = false, other = false;
  cout << "\nTransaction: request to allocate " << size << " bytes for process " << processId << ", block ID: " << blockId << endl; 
  if (whichAlgorithm == "B")
  {
    for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++) 
    {
      if((iteratorA->size - size) < temporarySize && (iteratorA->size - size) > 0)
      {
        temporarySize = (iteratorA->size - size);
        iteratorB = iteratorA;
        found = true;
        other = true;
      }
    }
    if (other == true)
    {
      for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++)
      {
        if (iteratorA->startingAddress == iteratorB->startingAddress)
        {
          iteratorA->size -= size;
          startingAddress = iteratorA->startingAddress;
          iteratorA->startingAddress += size;
          inUseBlocks.push_front(Block(startingAddress, size, processId, blockId)); 
          break;
        }
      }
    }
  }
  else if (whichAlgorithm == "F")
  {
    for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++)
    {
      if ((iteratorA->size - size) >= 0)
      {
        cout << "Found a block of size " << iteratorA->size << endl;
        iteratorA->size -= size;
        startingAddress = iteratorA->startingAddress;
        iteratorA->startingAddress += size;
        inUseBlocks.push_front(Block(iteratorA->startingAddress, size, processId, blockId));
        found = true;
        break;
      }
    }
  }
  if (found == false)
  {
    cout << "There is no sufficiently large block." << endl;
  }
  else
  {
    cout << "Success in allocating a block" << endl;
  }
  printStatus(false);
}
 
void deallocate(int processId, string blockId)
{
 // pushing a new block to the available blocks
  // removing something from in use
  //
  // that means, i need to take the block from in use, 
  // remove it from in use
  bool found = false;
  bool insert = false;
  list<Block>::iterator iteratorT, iteratorA, iteratorI;
  cout << "\nTransaction: request to deallocate block ID " << blockId << " for process " << processId << endl;

  for (iteratorI = inUseBlocks.begin(); iteratorI != inUseBlocks.end(); ++iteratorI)
  {
    if (iteratorI->processId == processId)
    {
      Block theBlock = Block(iteratorI->startingAddress, iteratorI->size, 0, "");
      list<Block>::iterator it;
      for (it = availableBlocks.begin(); it != availableBlocks.end(); it++)
      {
        if (it->startingAddress > iteratorI->startingAddress)
        {
          availableBlocks.insert(it, theBlock);
          insert = true;
          break;
        }
      }
      if (insert == false)
      {
        availableBlocks.push_back(theBlock);
      }
      for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++)
      {
        
        iteratorT = iteratorA;
        iteratorT++;
        
        if ((iteratorA->size + iteratorA->startingAddress) == iteratorT->startingAddress)
        {
          if ((iteratorT->size + iteratorA->size) <= megabyte *4)
          {
            iteratorA->size += iteratorT->size;
            availableBlocks.erase(iteratorT);
            iteratorA--;
          }
        }

      }
      // cout << "Merging two blocks at " << address << " and " << otherAddress << endl;
      inUseBlocks.erase(iteratorI);
      iteratorI--;
      found = true;
    }
  }
  if (found == false)
  {
    cout << "Unable to comply as the indicated process cannot be found." << endl;
  }
  else
  {
    cout << "Success in terminating a process" << endl;
  }

}
 
void terminate(int processId)
{
  // pushing a new block to the available blocks
  // removing something from in use
  //
  // that means, i need to take the block from in use, 
  // remove it from in use
  bool found = false;
  bool insert = false;
  list<Block>::iterator iteratorT, iteratorA, iteratorI;
  cout << "\nTransaction: request to terminate process " << processId << endl;

  for (iteratorI = inUseBlocks.begin(); iteratorI != inUseBlocks.end(); ++iteratorI)
  {
    if (iteratorI->processId == processId)
    {
      Block theBlock = Block(iteratorI->startingAddress, iteratorI->size, 0, "");
      list<Block>::iterator it;
      for (it = availableBlocks.begin(); it != availableBlocks.end(); it++)
      {
        if (it->startingAddress > iteratorI->startingAddress)
        {
          availableBlocks.insert(it, theBlock);
          insert = true;
          break;
        }
      }
      if (insert == false)
      {
        availableBlocks.push_back(theBlock);
      }
      for (iteratorA = availableBlocks.begin(); iteratorA != availableBlocks.end(); iteratorA++)
      {
        
        iteratorT = iteratorA;
        iteratorT++;
        
        if ((iteratorA->size + iteratorA->startingAddress) == iteratorT->startingAddress)
        {
          if ((iteratorT->size + iteratorA->size) <= megabyte *4)
          {
            iteratorA->size += iteratorT->size;
            availableBlocks.erase(iteratorT);
            iteratorA--;
          }
        }

      }
      // cout << "Merging two blocks at " << address << " and " << otherAddress << endl;
      inUseBlocks.erase(iteratorI);
      iteratorI--;
      found = true;
    }
  }
  if (found == false)
  {
    cout << "Unable to comply as the indicated process cannot be found." << endl;
  }
  else
  {
    cout << "Success in terminating a process" << endl;
  }
  printStatus(false);
}
 
void printStatus(bool transaction)
{
  // Print available blocks & size
  cout << "\nList of available blocks" << endl;
  availableSize = 0;
  inUseSize = 0;
  for (auto block : availableBlocks)
  {
    block.print(transaction);
    availableSize += block.size;
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
      block.print(true);
      inUseSize += block.size;
    }
  }
  cout << "Total size of the list = " << inUseSize << endl; 
 
  // Reset counters
  inUseSize = 0;
  availableSize = 0;
}
