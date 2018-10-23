/*

*/
#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <map>
#include <set>
#include <vector>

#define BITLENGTH 8

/* forward declaration */
class Node;

class FingerTable
{
public:
    /**
     * @param nodeId: the id of node hosting the finger table.
     */
    FingerTable(uint8_t nodeId);
    void set(size_t index, Node* successor) {
        fingerTable_[index] = successor;
    }
    uint8_t get(size_t index) {
        return 0; //fingerTable_[index];
    }
    // TODO: shrbs: complete print function
    void prettyPrint();

private:
    uint8_t nodeId_;
    std::vector<Node*> fingerTable_;
};


FingerTable::FingerTable(uint8_t nodeId): nodeId_(nodeId) {
    /* According to Chord paper, the finger table starts from index=1 */
    fingerTable_.resize(BITLENGTH + 1);
}

class Node
{
public:
    //Node(uint8_t id): id_(id){}
    //TODO: shrbs: implement node join function
    /**
     * @param node: the first node to contact with to initialize join process. If this is the first node to join the Chord network, the parameter is NULL.
     */
    void                        join(Node* node);
    //TODO: shrbs: implement DHT lookup
    uint8_t                     find(uint8_t key);
    //TODO: shrbs: implement DHT key insertion
    void                        insert(uint8_t key);
    //TODO: shrbs: implement DHT key deletion
    void                        remove(uint8_t key);

private:
    uint64_t                    id_;
    FingerTable                 fingerTable_;
    std::map<uint8_t, uint8_t> localKeys_;
};

#endif

