#ifndef FINGERTABLE_H
#define FINGERTABLE_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <iostream>
#include <cmath>
#define BITLENGTH 8
//FIXME: 2 power 8 
#define TOTALNODE 256 
//TESTME: tested for BITLENGTH 8
#define CHORDJUMP(nodeindex, logdist) nodeindex + ((2 << logdist) % (2<< (BITLENGTH - 1)))

using namespace std;

/* forward declaration */
class Node;

class FingerTable
{
public:
    FingerTable () {}
    FingerTable(uint8_t nodeId);
    void set(size_t index, Node* successor)
    {
        vecfingerTable_[index] = successor;
    }
    Node* get(size_t index)
    {
        return vecfingerTable_[index];
    }
    uint8_t get_size()
    {
        return fingerTableSize;
    }
    // TODO: shrbs: complete print function
    void prettyPrint();
    void copyFingerTable(FingerTable srcft);
    

private:
    uint8_t nodeId_;
    uint8_t fingerTableSize;
    std::vector<Node*> vecfingerTable_;
};



#endif