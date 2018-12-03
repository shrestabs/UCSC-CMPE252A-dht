/**
 * @file               node.h
 * @brief              Defines the node class that represents a server in a DHT.
 * 
 * A Node represents a server in a DHT. It saves files in it. For the scope of 
 * this project both the node saves files represented by 
 *  
 * @author             Shresta B.S.
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
        myfingerTable_          = FingerTable(id);
        pred_id                 = id;
        pred_address            = this;

        DEBUG("Node constructed"<<endl);
        DEBUG("Number of keys at node "<<unsigned(id)<<
              " - init - "<<localKeys_.size()<<"-----"<<endl);
    }
    
    void                        join(Node* node);
    bool                        find(uint8_t key);
    void                        insert(uint8_t key, uint8_t value);
    void                        remove(uint8_t key);
    void                        printKeys(Node *srcnode);
    bool                        accessKeyRPC(uint8_t key, Node *);
    void                        initNodeFingertable(Node* bootstrapper);
    void                        updateNodeFingerTable(void);
    void                        updateThisNodesFingerTable(Node *update, 
                                                           uint8_t updateid, 
                                                           uint8_t index);
    Node*                       remoteRecursiveLookup(uint8_t key);
    void                        moveKeys(Node *srcnode, Node *nodedest);
    int                         sendStablizeMessage(Node *successornode, 
                                                    uint8_t curid_);
    uint8_t                     sendNotifyMessage();
    bool                        cycled(uint8_t update_id, uint8_t index);

private:
    uint8_t                     id_;            /* id of this node */
    uint8_t                     pred_id;        /* id of the predecessor node */
    Node*                       pred_address;   /* Address of the predecessor 
                                                    node */
    FingerTable                 myfingerTable_; /* Fingertable to successors to 
                                                    lookup, from node */
    std::map<uint8_t, uint8_t> localKeys_;      /* Denotes files on server */
};

#endif      /* NODE_H */

