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
#include <unistd.h> /*delete this*/

/*
    Perodically send stablize message to successor. Since we do not
    have a timer implementation we send stabilize after join operation
    because we know its time for successor to know its new pred.  
*/

int
Node::sendStablizeMessage(Node *successornode, uint8_t predid_)
 {
    /* if the node that sent stabilize has a (normalized) id greater 
        than current pred value, update the pred. result this node id 
        to new pred. Also return pred to any node that sends it stablize
    */
   successornode->pred_id = predid_;
   return successornode->sendNotifyMessage();
 }

/* 
    Notifiy message is sent in response to the node that send the stabilize 
    message. With the response from the 
*/
uint8_t
Node::sendNotifyMessage()
 {
    return pred_id;
 }

/* 
    @details             Given a key and node this function accesses 
                        the node(map) and returns the content (value) 
                        from that node
    @return             -1 on failure, value on success
 */
 bool
 Node::accessKeyRPC(uint8_t key, Node* node)
 {
    int value = 0;
    std::map<uint8_t, uint8_t>::iterator it;
    it = node->localKeys_.find(key);
    if (it != node->localKeys_.end()) {
        cout<<"accessKeyRPC DEBUG: key"<<unsigned(node->localKeys_[key])<<" found at "<<unsigned(id_)<<endl;
        return  1;
    }
     return value;
 }

void
Node::initNodeFingertable()
{
    cout<<__FUNCTION__<<": filling this &="<<this<<endl;
    for(uint8_t i=0; i<myfingerTable_.get_size(); i++) {
        myfingerTable_.set(i, this, id_);
    }
}

/*
    @details     O(log N) lookup. 
                ith entry at peer with id n is first peer with id >= n + 2^i * (mod 2^m)
                Recur and reach predecessor. 
    @return     successor
    */
Node* 
Node::remoteRecursiveLookup(uint8_t key)
{
    cout<<"DEBUG: Key is "<<unsigned(key)<<" this NodeID,successor id "<<unsigned(id_)<<" "<<unsigned(successor_id)<<endl;
    /* case 1: only 1 server in ring */
    if ((successor_id == id_) || (key == id_)) /* if (pred = id) implicit*/
        return this;
    int normpred, normkey;
    int normid = NORMFACTOR;
    if(pred_id < id_)
        normpred = NORMFACTOR - pred_id;
    else
        normpred = NORMFACTOR - ((255 - pred_id) + id_);

    int keydist = max(abs(key - id_), abs(id_ + 255 - key));
    if(key < id_)
        normkey = NORMFACTOR - key;
    else
        normkey = NORMFACTOR + key;

    if ((normpred < normkey) && (normid > normkey)) {
        cout<<"In this node normpred normkey"<<normpred<<normkey<<endl;
        return this;
    }

    Node *nextnode = NULL;
    for(uint8_t i=0; i< myfingerTable_.get_size() - 2 ; i++) {
        if( (normkey >= normid + myfingerTable_.get(i)->id_ ) && 
            (normkey <=  normid + myfingerTable_.get(i+1)->id_ ) ) { //FIXME same 3 cases apply here 
            nextnode = this->myfingerTable_.get(i);
            cout<<"DEBUGREC"<<"Next iter "<<unsigned(nextnode->id_)<<endl;
            sleep(1);
            return nextnode->remoteRecursiveLookup(key);
        }
        else {
            cout<<"ERROR caught"<<endl;
            sleep(1);
        }
    }
    /* Worst case: if you reached here after vecfingerTable_[BITLENGTH]
        recur for last entry */
    cout<<"For last entry rec"<<endl;
    nextnode = myfingerTable_.get(BITLENGTH - 1);
    return nextnode->remoteRecursiveLookup(key);
}

void
Node::join(Node* node)
{
    /* Based on key id posnode is where (after pred) the new node joins. */
    Node *sucpos = NULL;
    /* First node of DHT */
    if(node == NULL) {
        initNodeFingertable();
        return;
    }
    /* Subsequent nodes. */
    else {
        /* Find the correct place to insert */
        sucpos = node->remoteRecursiveLookup(id_);
        cout<<"found successor_id "<<unsigned(sucpos->id_)<<std::flush<<endl;
        successor_id = sucpos->id_; /* pred entry is still stale */
        pred_id = sendStablizeMessage(sucpos, id_);
        sucpos->myfingerTable_.updateFingerTable(this, id_, pred_id , id_);
        /* copy fingertable from bootstrap node */
        myfingerTable_.copyFingerTable(id_, successor_id, predpos->myfingerTable_);
        /* move keys */
        cout<<"Updated successor_id of "<<unsigned(id_)<<"to "<<successor_id<<std::flush; 
        moveKeys(predpos, this);
    }
/* build your finger table. bootstrap it from the node introducing it to you */
/* move 1/n keys */
/* print migrated keys */
    cout<<"Finger table of joiner(dest) updated"<<endl;
    myfingerTable_.prettyPrint();
    cout<<"Finger table of bootstrapper(src) updated"<<endl;
    node->myfingerTable_.prettyPrint();
    /* Once a node is joined stablize all nodes to update fingertable */
    stablizeNodes();
    return;
}

/* @details     Finds where the key is from the fingertable and 
                queries that node for the value with the key
    @return     -1 if found value otherwise
*/
bool
Node::find(uint8_t key)
{
    /* print sequence of nodes to talk to */
    cout<<"Search request for key="<<unsigned(key)<<endl;
    Node *foundnode = remoteRecursiveLookup(key);
    if (accessKeyRPC(key, foundnode))
        return 1;
    return 0;

}

void
Node::remove(uint8_t key)
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
        cout<<"Element found.(key,value)="<<unsigned(it->first)<<" ";
        cout<<unsigned(it->second)<<" at NodeID="<<foundnode->id_;
        foundnode->localKeys_.erase(it);
        return;
    }
    cout<<"No delete performed. Element not found in the network "<<endl;
    return;
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

/*
    @details    
    @return     return the position of the predecessor
*/
void
Node::moveKeys(Node *srcnode, Node *nodedest)
{
    Node* pred = 0;
    map<uint8_t, uint8_t>::iterator it;
    for(it=nodedest->localKeys_.begin(); it != nodedest->localKeys_.end(); it++) {
        if((it->first < srcnode->id_ ) && (it->first > nodedest->id_)) {
            continue;
        }
        else {
            cout<<"Moved key (key,value)="<<unsigned(it->first)<<" ";
            cout<<unsigned(it->second);
            cout<<" from NodeID="<<srcnode->id_<<" to "<<nodedest->id_<<endl;
            srcnode->localKeys_.insert(std::make_pair(it->first, it->second));
            nodedest->localKeys_.erase(it);
        }
    }
}