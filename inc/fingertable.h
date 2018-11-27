#ifndef FINGERTABLE_H
#define FINGERTABLE_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <iostream>
#include <cmath>
#define BITLENGTH 8
#define NORMFACTOR 1000
#define CHORDJUMP(nodeindex, logdist) (nodeindex + (1 << logdist) ) % (1<< (BITLENGTH))

using namespace std;

/* forward declaration */
class Node;

class FingerTable
{
public:
    FingerTable () {/*unused*/}
    FingerTable(uint8_t nodeId);
    void set(uint8_t index, Node* nodepointer, uint8_t nodeid)
    {
        vecfingerTable_[index].first = nodepointer;
        vecfingerTable_[index].second = nodeid;
    }
    Node* get(size_t index)
    {
        return vecfingerTable_[index].first;
    }
    uint8_t get_size()
    {
        return fingerTableSize;
    }
    // TODO: decorate print function
    void prettyPrint();
    //void copyFingerTable(uint8_t id, uint8_t successor_id, Node* pred);
    

private:
    uint8_t nodeId_;
    uint8_t fingerTableSize;
    std::vector<pair<Node*, uint8_t> > vecfingerTable_;
};



#endif