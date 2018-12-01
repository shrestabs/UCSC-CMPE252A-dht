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
        myfingerTable_= FingerTable(id);
        pred_id = id;
        pred_address = this;
        successor_id = id; /* For stablize */
        cout<<"Node constructed"<<endl;
        cout<<"Number of keys at node "<<unsigned(id)<<" - init - "<<localKeys_.size()<<"-----"<<endl;
    }

    uint8_t getid();


    /**
     * @param node: the first node to contact with to initialize join process. 
     * If this is the first node to join the Chord network, the parameter is NULL.
     */
    void                        join(Node* node);
    bool                         find(uint8_t key);
    void                        insert(uint8_t key, uint8_t value);
    void                        remove(uint8_t key);
    bool                        accessKeyRPC(uint8_t key, Node *);
    void                        initNodeFingertable(Node* bootstrapper);
    void                        updateNodeFingerTable(Node* bootstrapper);
    void                        updateThisNodesFingerTable(Node *update, uint8_t updateid, uint8_t index);
    Node*                       remoteRecursiveLookup(uint8_t key);
    void                        moveKeys(Node *srcnode, Node *nodedest);
    int                         sendStablizeMessage(Node *successornode, uint8_t curid_);
    uint8_t                     sendNotifyMessage();
    bool                        cycled(uint8_t update_id, uint8_t index);
private:
    uint8_t                     id_;                /* id of this node */
    uint8_t                     pred_id;            /* id of the predecessor node */
    Node*                       pred_address;       /* Address of the predecessor node */
    uint8_t                     successor_id;       /* id of the successor node */
    FingerTable                 myfingerTable_;     /* Fingertable to 
                                                        lookup on this node*/
    std::map<uint8_t, uint8_t> localKeys_;          /* Denotes files on server */
};

#endif

