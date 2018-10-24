/*

*/
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <stdint.h>
#include <map>

#include "fingertable.h"
 

class Node
{
public:
    Node(uint8_t id): id_(id)
    {

    }
    /**
     * @param node: the first node to contact with to initialize join process. 
     * If this is the first node to join the Chord network, the parameter is NULL.
     */
    void                        join(Node* node);
    uint8_t                     find(uint8_t key);
    void                        insert(uint8_t key, uint8_t value);
    void                        remove(uint8_t key);

private:
    uint64_t                    id_;                /* id of this node */
    FingerTable                 fingerTable_;       /* Fingertable to 
                                                        lookup on this node*/
    std::map<uint8_t, uint8_t> localKeys_;          /* Denotes files on server */
};

#endif

