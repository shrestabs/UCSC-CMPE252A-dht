/**
 * @file           node.cpp
 * @brief          Implements the node of the distributed network.
 * 
 * Matains the true state of the node. Changes occur to the node when other new 
 * nodes join the network. Also files can be added, search from the network. 
 * These are represented by the operations. The key operations are join, insert 
 * and find that is exposed. Other operations like update is internal to the 
 * maintenance of the protocol.
 * 
 * @author         Shresta B.S
 * @date           23/Oct/2018
 */
#include "node.h"

/**
 * @fn              accessKeyRPC
 *
 * Given a key and node this function accesses the node(map) and returns the 
 * content (value) from that node
 *
 * @param[in]       key the key of the file to be accessed
 * @param[in]       node the node which is supposed to have the key
 * @return          0 on failure, 1 on success
 */
 bool
 Node::accessKeyRPC(uint8_t key, Node* node)
 {
    int value = 0;
    std::map<uint8_t, uint8_t>::iterator it;
    it = node->localKeys_.find(key);
    if (it != node->localKeys_.end()) {
        DEBUG("DEBUG: accessKeyRPC key"<<unsigned(node->localKeys_[key])<<
              " found at "<<unsigned(id_)<<endl);
        return  1;
    }
     return value;
 }

/**
 * @fn              initNodeFingertable
 *
 * When a new node joins the network, it needs to be initialized with its pre-
 * decessor and successor fingertable.
 *
 * @param[in]       bootstrapper the node from which the current node being 
 *                  initialised gets information about the current state 
 *                  of the network
 * @return          Void
 */
void
Node::initNodeFingertable(Node* bootstrapper)
{
    DEBUG("DEBUG"<<"init of subsequent node of network "<<unsigned(id_)<<endl);
    Node* tmpnode = bootstrapper->remoteRecursiveLookup(CHORDJUMP(id_, 0));
    cout<<"returned successor "<<unsigned(tmpnode->id_)<<endl;
    myfingerTable_.set(0, tmpnode, tmpnode->id_);

    int normtempid = tmpnode->id_;
    if (tmpnode->id_ <= id_) {
            normtempid += MAXINDEX; /* id_ + (255 - id_) */ 
            DEBUG(__FUNCTION__<<" normalized succid "<<normtempid<<endl);
    }
    for(uint8_t i=1; i< myfingerTable_.get_size() ; i++) {
        Node *tmpnode1 = NULL;
        int normchordjump = CHORDJUMP(id_, i);
        if (normchordjump <= id_) {
            normchordjump += MAXINDEX; /* id_ + (255 - id_) */ 
            DEBUG(__FUNCTION__<<" normalized normchordjump "
                <<normchordjump<<endl);
        }

        if (normtempid >= normchordjump ) {
            DEBUG("reusing prev ith value for "<<unsigned(i)<<endl);
            tmpnode1 = myfingerTable_.get(i-1);
        }
        else {
            DEBUG("DEBUG:else part: prev i entry outdated "<<tmpnode<<endl);
            tmpnode1 = bootstrapper->remoteRecursiveLookup(CHORDJUMP(id_, i));
        }
        myfingerTable_.set(i, tmpnode1, tmpnode1->id_);
    }

    pred_id = tmpnode->pred_id;
    cout<<"Updated pred_id to "<<unsigned(pred_id)<<endl;
    pred_address = tmpnode->pred_address;
    tmpnode->pred_id = id_;
    cout<<"Updated "<<unsigned(tmpnode->id_)<<" predid to "
        <<unsigned(id_)<<endl;
    tmpnode->pred_address = this;

#ifdef _DEBUG
    myfingerTable_.prettyPrint();
#endif
}

/**
 * @fn              find
 *
 * Figures which node has the key in the network and calls accessKeyRPC to look
 * into the particular node
 *
 * @param[in]       key the to be searched for in the network
 * @return          0 on failure, 1 on success
 */
bool
Node::find(uint8_t key)
{
    /*
     * print sequence of nodes to talk to 
     */
    cout<<"Search request for key="<<unsigned(key)<<endl;
    Node *foundnode = remoteRecursiveLookup(key);
    if (accessKeyRPC(key, foundnode))
        return 1;
    return 0;
}

/**
 * @fn              remove
 * 
 * Removes a key (that represents a file) from the chord network
 *
 * @param[in]       key the key of the file to be removed
 * @return          Void
 */
void
Node::remove(uint8_t key)
{
    /* 
     * Validate input
     */
    if( (key <0)||(key > MAXINDEX) ) {
        cout<<key<<"ERROR: Bad key"<<endl;
        return;
    }
    map<uint8_t, uint8_t>::iterator it;
    /*
     * Check fingertable of the node
     */
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

/**
 * @fn              insert
 * 
 * Adding data to the distribued table. in the real work key, value can 
 * be key,music.mp3 or key,book.pdf
 *
 * @param[in]       key the key of the file to be removed
 * @return          Void
 * @bugs            None
 */ 
void
Node::insert(uint8_t key, uint8_t value)
{
    /* Validate input */
    if( (key <0)||(key > MAXINDEX) ) {
        cout<<key<<"ERROR: Bad key"<<endl;
        return;
    }
    map<uint8_t, uint8_t>::iterator it;
    /*
     * Check fingertable of the node
     */
    Node *foundnode = remoteRecursiveLookup(key);
    it = foundnode->localKeys_.find(key);
    if (it != foundnode->localKeys_.end()) {
        cout<<"Element already exists with value "
            <<unsigned(foundnode->localKeys_[key])<<endl;
        cout<<"Update not supported. Returning"<<endl;
        return;
    }
    foundnode->localKeys_.insert(std::make_pair(key,value));
    cout<<"(Key, value)=("<<unsigned(key)<<",";
    cout<<unsigned(value)<<") inserted at Node id = "
        <<unsigned(foundnode->id_)<<endl;
    return;
}

/**
 * @fn              movekeys
 *
 * After a new node has joined 1/N of the nodes are reshuffled. The keys or file
 * are moved from the immediate successor to this node
 *
 * @param[in]       srcnode successor node from which keys are taken
 * @param[in]       nodedest destination node to which nodes are moved to
 * @return          Void
 * @bugs            range before and after MAXINDEX not compared before copy
 */
void
Node::moveKeys(Node *srcnode, Node *nodedest)
{
    Node* pred = 0;
    map<uint8_t, uint8_t>::iterator it;
    for(it=nodedest->localKeys_.begin(); 
        it != nodedest->localKeys_.end(); it++) {
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

/**
 * @fn              join
 *
 * Implemented from the pseudocode in the chord paper. Updates the state of 
 * other nodes and this node to accomodate the join of a new node. If this is 
 * the first node to join the Chord network, then node param is NULL.
 *
 * @param[in]       node The bootstrapper of the new node
 * @return          Void
 */
void
Node::join(Node* node)
{
    Node *sucpos = NULL;
    /* 
     * First node of DHT
     */
    if(node == NULL) {
        cout<<" init of first node of network "<<unsigned(id_)<<endl;
        for(uint8_t i=0; i< myfingerTable_.get_size() ; i++) {
            myfingerTable_.set(i, this, id_);
        }
        myfingerTable_.prettyPrint();
        return;
    }
    /* 
     * Subsequent nodes. Find the correct place to insert. Based on key id 
     * sucpos is where (after pred) the new node joins.
     */
    sucpos = node->remoteRecursiveLookup(id_);
    /* initfingertable from pred            */
    initNodeFingertable(sucpos);
    /* update fingertable of related nodes  */
    updateNodeFingerTable();
    /* move keys                            */
    moveKeys(sucpos->pred_address, this);
    cout<<"Finger table of joiner(dest) updated"<<endl;
    myfingerTable_.prettyPrint();
    cout<<"Finger table of bootstrapper(src) updated"<<endl;
    node->myfingerTable_.prettyPrint();
    return;
}

/**
 * @fn              remoteRecursiveLookup
 *
 * Performs a O(log N) lookup. ith entry at peer with id n is first peer with 
 * id >= (n + 2^i ) mod 2^m . Recur and reach predecessor. 
 *
 * @param[in]       key The key to be recurrsively looked in the network
 * @return          Node pointer to the node that has to key
 */ 
Node* 
Node::remoteRecursiveLookup(uint8_t key)
{
    DEBUG(__FUNCTION__<<"DEBUG: pred id key "<<unsigned(pred_id)<<" "
        <<unsigned(id_)<<" "<<unsigned(key)<<endl);
    /* 
     * Base case for recursion 
     */
    if (pred_id == id_)
        return this;

    if((key > pred_id) && (key <= id_)) {
        DEBUG("found in this node "<<unsigned(id_)<<endl);
        return this;
    }

    if(pred_id > id_) {
        if((key > pred_id) || (key <= id_)) {
            DEBUG("found in this node "<<unsigned(id_)<<endl);
            return this;
        }
    }

    /* 
     * Look at finger table now. Recur now 
     */
    for(uint8_t i=0; i< myfingerTable_.get_size() - 1 ; i++) {
        int normsucc0 = CHORDJUMP(id_, i);
        int normsucc1 = CHORDJUMP(id_, (i+1));
        DEBUG("normsucc0 "<< normsucc0 <<" key "<<unsigned(key)
            <<" normsucc1 "<<normsucc1<<endl);

        if((key >= normsucc0) && (key < normsucc1))
            return myfingerTable_.get(i)->remoteRecursiveLookup(key); 
        /* across MAXINDEX boundary */
        if (normsucc0 > normsucc1) {
            if((key >= normsucc0) || (key < normsucc1))
                return (myfingerTable_.get(i))->remoteRecursiveLookup(key);
        }
    }
    DEBUG(<<"Last index recur "<<endl);
    return (myfingerTable_.get(7))->remoteRecursiveLookup(key);
}

/**
 * @fn              updateNodeFingerTable
 *
 * Reflect the joining of a new node on its predecessor so that the predecessor 
 * update their successor fingertable. 
 *
 * @param[in]       none 
 * @return          void
 */
void
Node::updateNodeFingerTable(void)
{
    DEBUG("---------------UPDATE STARTED -----------------"<<endl);
    for(int i=0; i<BITLENGTH; i++) {
        Node* succ = remoteRecursiveLookup(id_ - ( 1 << (i) ));
        uint8_t temp = id_ - ( 1 << (i));
        DEBUG(<<__FUNCTION__<<" for "<<i<<" succ found is "<<
              unsigned(succ->id_)<<"id_ - ( 1 << (i) "<< unsigned(temp) <<endl);
        (succ->pred_address)->updateThisNodesFingerTable(this, id_, i);
    }
}

/**
 * @fn              updateNodeFingerTable 
 * 
 * For a jump behind of 2^i recur till all predecessors for a particular i has 
 * the update about the node of the node
 * 
 * @param[in]       update the pointer to the new node
 * @param[in]       updateid the id of the new node
 * index            the ith jump behind from the new node
 * @return          void
 */
void
Node::updateThisNodesFingerTable(Node *update, uint8_t updateid, uint8_t index)
{

    DEBUG(__FUNCTION__<<" DEBUG:"<<unsigned(index)<<" "<<unsigned(id_)<<" "
        <<unsigned(updateid)<<" "
        <<unsigned(myfingerTable_.get(index)->id_)<<endl);
    int normftindex = myfingerTable_.get(index)->id_;
    Node *ftnode = (myfingerTable_.get(index))->pred_address;
    int normupdate = updateid;
    /*
     * eg case1. pred or id = 254 updateid=2 other succ 3, 4
     *    case2. updateid 63 id 5
     */
    if (updateid <= id_) { 
        DEBUG("using MAXINDEX boundary logic"<<endl); 
        /*
         * Dont just rearrange on number line . keep them at 
         * the rite distance from one another
         */
        normupdate += (MAXINDEX - id_);
    }
    /* 
     * we jumped accross the MAXINDEX bound 
     */
    if ( id_ >= normftindex) {
            DEBUG("~~~~~~~~~~~~~normindex"<<endl);
            normftindex += (MAXINDEX - id_);
    }
    if ((normupdate >= id_) && (normupdate < normftindex )) {
        DEBUG("~~~~~~~~~~~~~update~~~~~~~~"<<endl);
        myfingerTable_.set(index, update, updateid);
        myfingerTable_.prettyPrint();
        DEBUG("~~~~~~~~~~~~~/update~~~~~~~~"<<endl);
        ftnode->updateThisNodesFingerTable(update, updateid, index);
    }
}