/**
 * @file               fingertable.h
 * @brief              Defines the fingertable class. 
 * 
 * Fingertable is Chord's way of ensuring nodes reachability to other nodes. It 
 * mantains pointers to successors. The vector of successors is stored as 
 * <Address, ID> pairs. Each node (server) has a fingertable within.
 *  
 * @author             Shresta B.S.
 * @bug                No know bugs.
*/
#ifndef FINGERTABLE_H
#define FINGERTABLE_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <iostream>
#include <cmath>

#define BITLENGTH 8
#define MAXINDEX 255
#define CHORDJUMP(nodeindex, logdist) \
        (nodeindex + (1 << logdist) ) % (1<< (BITLENGTH))

#ifdef _DEBUG
#define DEBUG(input) std::cout << (input);
#else
#define DEBUG(input)
#endif

using namespace std;

/* forward declaration */
class Node;

class FingerTable
{
public:
    FingerTable () {}
    FingerTable(uint8_t nodeId);
    void set(uint8_t index, Node* nodepointer, uint8_t nodeid)
    {
        vecfingerTable_[index].first    = nodepointer;
        vecfingerTable_[index].second   = nodeid;
    }
    Node* get(size_t index)
    {
        return vecfingerTable_[index].first;
    }
    uint8_t get_size(void)
    {
        return fingerTableSize;
    }
    void                                prettyPrint();    

private:
    uint8_t                             nodeId_;
    uint8_t                             fingerTableSize;
    std::vector<pair<Node*, uint8_t> >  vecfingerTable_;
};

#endif  /* FINGERTABLE_H */