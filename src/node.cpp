/*
@file           node.cpp
@brief          Implements the node of the distributed networks.

@details        All nodes APIs are exposed to the caller. Calls fingertable
                APIs underneath whose implementation is abstracted from the 
                user
@author         Shresta B.S
@date           23/Oct/2018
*/

#include "node.h"

int
Node::stablizeNode()
 {
    return 0;
 }

/* 
    @details             Given a key and node this function accesses 
                        the node(map) and returns the content (value) 
                        from that node
    @return             -1 on failure, value on success
 */
 int
 Node::accessKeyRPC(uint8_t key, Node* node)
 {
    int value = -1;
    std::map<uint8_t, uint8_t>::iterator it;
    it = node->localKeys_.find(key);
    if (it != node->localKeys_.end()) {
        cout<<"accessKeyRPC DEBUG: key"<<unsigned(node->localKeys_[key])<<" found at "<<unsigned(id_)<<endl;
        return  node->localKeys_[key];
    }
     return value;
 }

void
Node::initNodeFingertable(Node* node)
{
    for(uint8_t i=0; i<myfingerTable_.get_size(); i++) {
        myfingerTable_.set(i, node);
    }
}

void
Node::printNodeFingertable(Node* node)
{
    for(uint8_t i=0; i<myfingerTable_.get_size() ; i++) {
        if(myfingerTable_.get(i) != NULL)
            cout<<unsigned(myfingerTable_.get(i)->id_)<<endl;
        else
            cout<<"Corrupted fingertable"<<endl;
    }
}

/*
    @details     O(log N) lookup. 
                ith entry at peer with id n is first peer with id >= n + 2^i * (mod 2^m)
    */
Node* 
Node::remoteRecursiveLookup(uint8_t key)
{
    cout<<"DEBUG: Key is "<<unsigned(key)<<" this->id_ is "<<unsigned(this->id_)<<endl;
    /* Base case 1: If 0th and nth entry are same, there is only 
       1 server in ring 
    */
    if(this->myfingerTable_.get(0)->id_ == 
       this->myfingerTable_.get(BITLENGTH - 1)->id_) {
        cout<<"DEBUG: base case1"<<endl;
        return this;
    }
    /* Base case 2: Search for key in current table */
    if( ((key >= this->id_) && (key < this->myfingerTable_.get(0)->id_)) ||
        ((key >= this->id_) && (key > this->myfingerTable_.get(0)->id_)) ) {
        return  this; 
    }
    Node *nextnode = NULL;
    for(uint8_t i=0; i<this->myfingerTable_.get_size(); i++) {
        if( (key >= this->myfingerTable_.get(i)->id_ ) && 
            (key < this->myfingerTable_.get(i+1)->id_ ) ) {
            nextnode = this->myfingerTable_.get(i);
            return nextnode->remoteRecursiveLookup(key);
        }
        else {
            cout<<"ERROR caught"<<endl;
        }
    }
    /* Worst case: if you reached here after vecfingerTable_[BITLENGTH]
        recur for last entry */
    nextnode = myfingerTable_.get(BITLENGTH - 1);
    return nextnode->remoteRecursiveLookup(key);
}

void
Node::join(Node* node)
{
    /* First node of DHT */
    if(node == NULL) {
        initNodeFingertable(this);
    }
    /* Subsequent nodes. */
    else {
        Node *posnode = remoteRecursiveLookup(id_);
        myfingerTable_.copyFingerTable(node->myfingerTable_);
        localKeys_.insert(node->localKeys_.begin(), node->localKeys_.end());
    }
/* build your finger table. bootstrap it from the node introducing it to you */
/* move 1/n keys */
/* print migrated keys */
    cout<<"Finger table updated"<<endl;
    printNodeFingertable(this);
}

/* @details     Finds where the key is from the fingertable and 
                queries that node for the value with the key
    @return     -1 if found value otherwise
*/
int
Node::find(uint8_t key)
{
    /* print sequence of nodes to talk to */
    cout<<"Search request for key="<<unsigned(key)<<endl;
    Node *foundnode = remoteRecursiveLookup(key);
    accessKeyRPC(key, foundnode);
    return 0;
}

void
Node::remove(uint8_t key)
{
    cout<<"Cannot exit the network. Feature not implemented :) "<<endl;
}

/*
    @detail         Adding data to the distribued table. in the real work key,
                    value can be key,music.mp3 or key,book.pdf
*/
void
Node::insert(uint8_t key, uint8_t value)
{
    /* Validate input */
    if( (key <0)||(key > 255) ) {
        cout<<key<<"ERROR: Bad key"<<endl;
        return;
    }
    map<uint8_t, uint8_t>::iterator it;
    /* Check fingertable of the node */
    Node *foundnode = remoteRecursiveLookup(key);
    it = foundnode->localKeys_.find(key);
    if (it != foundnode->localKeys_.end()) {
        cout<<"Element already exists with value "<<unsigned(foundnode->localKeys_[key])<<endl;
        cout<<"Update not supported. Returning"<<endl;
        return;
    }
    foundnode->localKeys_.insert(std::make_pair(key,value));
    cout<<"(Key, value)=("<<unsigned(key)<<",";
    cout<<unsigned(value)<<") inserted at Node id = "<<unsigned(foundnode->id_)<<endl;
    return;
}