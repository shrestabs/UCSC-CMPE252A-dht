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
Node::initNodeFingertable(Node* bootstrapper)
{
    cout<<"DEBUG"<<"init of subsequent node of network "<<unsigned(id_)<<endl;
    Node* tmpnode = bootstrapper->remoteRecursiveLookup(CHORDJUMP(id_, 0));
    cout<<"returned successor "<<unsigned(tmpnode->id_)<<endl;
    myfingerTable_.set(0, tmpnode, tmpnode->id_);

    pred_id = tmpnode->pred_id;
    cout<<"Updated pred_id to "<<unsigned(pred_id)<<endl;
    pred_address = tmpnode->pred_address;
    tmpnode->pred_id = id_;
    cout<<"Updated "<<unsigned(tmpnode->id_)<<" predid to "<<unsigned(id_)<<endl;
    tmpnode->pred_address = this;

    
    int normtempid = tmpnode->id_;
    if (tmpnode->id_ < id_) {
            normtempid += 255;
            cout<<__FUNCTION__<<" normalized succid "<<normtempid<<endl;
    }
    for(uint8_t i=1; i< myfingerTable_.get_size() ; i++) {
        Node *tmpnode1 = NULL;
        int normchordjump = CHORDJUMP(id_, i);
        if (normchordjump < id_) {
            normchordjump += 255;
            cout<<__FUNCTION__<<" normalized normchordjump "<<normchordjump<<endl;
        }

        if (normtempid > normchordjump ) { //FIXME: RANGE 
            cout<<"reusing prev ith value for "<<unsigned(i)<<endl;
            tmpnode1 = myfingerTable_.get(i-1);
        }
        else {
            cout<<"DEBUG:else part: prev i entry outdated "<<tmpnode<<endl;
            tmpnode1 = bootstrapper->remoteRecursiveLookup(CHORDJUMP(id_, i));
        }
        myfingerTable_.set(i, tmpnode1, tmpnode1->id_);
    }
    myfingerTable_.prettyPrint();
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
    //FIXME: range before and after 255 before compare and copy */
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



bool 
Node::cycled(uint8_t updateid, uint8_t index)
{
    int sum = id_;
    for(int i=0; i<index; i++) {
        sum += 1<<i;
        if(sum > updateid) 
            return true;
        
    }
    return false;
}





void
Node::join(Node* node)
{
    /* Based on key id posnode is where (after pred) the new node joins. */
    Node *sucpos = NULL;
    /* First node of DHT */
    if(node == NULL) {
        cout<<"DEBUG"<<" init of first node of network "<<unsigned(id_)<<endl;
        for(uint8_t i=0; i< myfingerTable_.get_size() ; i++) {
            myfingerTable_.set(i, this, id_);
        }
        myfingerTable_.prettyPrint();
        return;
    }
    /* Subsequent nodes. */
    /* Find the correct place to insert */
    sucpos = node->remoteRecursiveLookup(id_);


    /* initfingertable from pred */
    initNodeFingertable(sucpos);
    /* update fingertable of related nodes */
    updateNodeFingerTable(node);

///////////////////////////////outdated logic 
    cout<<"found successor_id "<<unsigned(sucpos->id_)<<std::flush<<endl;
    successor_id = sucpos->id_;
    cout<<"Updated successor_id of "<<unsigned(id_)<<" to "<<unsigned(successor_id)<<std::flush<<endl; 
    
    //pred_id = sendStablizeMessage(sucpos, id_);
///////////////////////////////outdated logic


    /* move keys */
    moveKeys(sucpos->pred_address, this);
    
/* build your finger table. bootstrap it from the node introducing it to you */
/* move 1/n keys */
/* print migrated keys */
    cout<<"Finger table of joiner(dest) updated"<<endl;
    myfingerTable_.prettyPrint();
    cout<<"Finger table of bootstrapper(src) updated"<<endl;
    node->myfingerTable_.prettyPrint();
    /* Once a node is joined stablize all nodes to update fingertable */
    return;
}

/*
    @details     O(log N) lookup. 
                ith entry at peer with id n is first peer with id >= n + 2^i * (mod 2^m)
                Recur and reach predecessor. 
                Instead of returning predecessor as chord suggests we return the successor
    @return     successor
    */
   //FIXME: optimize this code to 1 block 
Node* 
Node::remoteRecursiveLookup(uint8_t key) // = find_predecessor in chord paper
{
    cout<<__FUNCTION__<<"DEBUG: pred id key "<<unsigned(pred_id)<<" "<<unsigned(id_)<<" "<<unsigned(key)<<endl;
    if (pred_id == id_) /* also (succ = id) implicit*/
        return this;

    if((key > pred_id) && (key <= id_)) {
        cout<<"1found in this node "<<unsigned(id_)<<endl;
        return this;
    }

    if(pred_id > id_) {
        if((key > pred_id) || (key <= id_)) {
            cout<<"2found in this node "<<unsigned(id_)<<endl;
            return this;
        }
    }

    /* Look at finger table now */
    for(uint8_t i=0; i< myfingerTable_.get_size() - 2 ; i++) {
        int normsucc0 = myfingerTable_.get(i)->id_;
        int normsucc1 = myfingerTable_.get(i+1)->id_;

        if((key > normsucc0) || (key <= normsucc1))
            return myfingerTable_.get(i+1)->remoteRecursiveLookup(key); 

        if (normsucc0 > normsucc1) {
            if((key > normsucc0) && (key <= normsucc1))
                return myfingerTable_.get(i+1)->remoteRecursiveLookup(key);
        }

    }
    cout<<"part last entry"<<endl;
    return myfingerTable_.get(7)->remoteRecursiveLookup(key);
 
}

void
Node::updateNodeFingerTable(Node *bootstrapper)
{
    cout<<"---------------UPDATE STARTED -----------------"<<endl;
    for(int i=0; i<BITLENGTH; i++) {
        Node* succ = remoteRecursiveLookup(id_ - ( 1 << (i) ));
        uint8_t temp = id_ - ( 1 << (i));  /* left shift does not gracefully mod on uint8_t  */
        //uint8_t temp = 
        cout<<__FUNCTION__<<" for "<<i<<" succ found is "<<unsigned(succ->id_)<<"id_ - ( 1 << (i) "<< unsigned(temp) <<endl;
        (succ->pred_address)->updateThisNodesFingerTable(this, id_, i);
    }
}

void
Node::updateThisNodesFingerTable(Node *update, uint8_t updateid, uint8_t index)
{

    cout<<__FUNCTION__<<" DEBUG:"<<unsigned(index)<<" "<<unsigned(id_)<<" "<<unsigned(updateid)<<" "<<unsigned(myfingerTable_.get(index)->id_)<<endl;
    int normftindex = myfingerTable_.get(index)->id_;
    Node *ftnode = (myfingerTable_.get(index))->pred_address;
    int normupdate = updateid;
    if (updateid <= id_) { //eg.pred or id= 254 updateid 2 other succ 3, 4,  or updateid 63 id 5
        cout<<"using 255 boundary logic"<<endl; 
        //updateid += 255;          bug is dont just rearrange. keep it at the rite distance from one another
        normupdate += (255 - id_);

    }
    
    /* we jumped accross the 255 bound */
    //if (cycled(updateid, index) ) {
    if ( id_ >= normftindex) {
            cout<<"~~~~~~~~~~~~~normindex"<<endl;
            normftindex += (255 - id_);
    }
    /* e.g. */
    if ((normupdate >= id_) && (normupdate < normftindex )) {
        cout<<"~~~~~~~~~~~~~update~~~~~~~~"<<endl;
        myfingerTable_.set(index, update, updateid);
        myfingerTable_.prettyPrint();
        cout<<"~~~~~~~~~~~~~/update~~~~~~~~"<<endl;
        ftnode->updateThisNodesFingerTable(update, updateid, index);
    }
}