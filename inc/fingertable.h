#ifndef FINGERTABLE_H
#define FINGERTABLE_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#define BITLENGTH 8

using namespace std;

/* forward declaration */
class Node;

class FingerTable
{
public:
    /**
     * @param nodeId: the id of node hosting the finger table.
     */
    FingerTable() {}
    FingerTable(uint8_t nodeId);
    void set(size_t index, Node* successor) {
        fingerTable_[index] = successor;
    }
    uint8_t get(size_t index) {
        return 0; //fingerTable_[index];
    }
    // TODO: shrbs: complete print function
    void prettyPrint();
    void stablizeNode();

private:
    uint8_t nodeId_;
    std::vector<Node*> fingerTable_;
};



#endif