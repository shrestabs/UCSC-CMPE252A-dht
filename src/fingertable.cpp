/*
@file           fingertable.cpp
@brief          Implements the node's fingertable.

@details        Each of the nodes in the system have their fingertable that 
                needs to be kept up to date. 
@author         Shresta B.S
@date           23/Oct/2018
*/
#include "fingertable.h"
FingerTable::FingerTable(uint8_t nodeId): nodeId_(nodeId)
{
    Node *dummy = NULL;
    cout<<"-----Constructor of fingertable"<<endl;
    fingerTableSize = BITLENGTH;
    /* According to Chord paper, the finger table starts from index=1 */
    for(uint8_t i=0; i<BITLENGTH ; i++) {
        vecfingerTable_.push_back(std::make_pair(dummy,i));
    }
    cout<<"DUMMY was "<<dummy<<endl;
}


void
FingerTable::prettyPrint()
{
    cout<<"Finger table print of "<<unsigned(nodeId_)<<endl;
    for(uint8_t i=0; i<vecfingerTable_.size(); i++) {
        if(vecfingerTable_[i].first == NULL) {
            cout<<"NULL"<<"\t";
        }
        else {
            cout<<"Address:";
            cout<<vecfingerTable_[i].first<<"\t";
        }

        cout<<unsigned(vecfingerTable_[i].second)<<"\n";

    }
    cout<<endl;
}
/*
    @details    when node with key is inserted predecessor table is modified
    @return     return the position of the predecessor
*/
void
FingerTable::updateFingerTable(Node *node, uint8_t thisnodeid, uint8_t pred_id, uint8_t key)
{
    /* Step 1: Normalize the circle to integer line */
    int normthisnodeid = NORMFACTOR; /* makes number line start at 1000 */
    int normpred, normkey_id;
    if(pred_id < thisnodeid)
        normpred = NORMFACTOR - pred_id;
    else
        normpred = NORMFACTOR - ((255 - pred_id) + id_);
        
    int normkey_id = NORMFACTOR - thisnodeid + key;

    cout<<"normthisnodeid normpred_id normkey_id"<<normthisnodeid<<" "<<normpred<<" "<<normkey_id<<endl;

    /* Step 2: Update Fingertable to reflect new server */
    for(uint8_t i=0; i<=vecfingerTable_.size() ; i++) {
        /* The earlier this loop ends, the distribution is more in DHT */
        if(( NORMFACTOR - thisnodeid + CHORDJUMP(thisnodeid, i) < normkey_id) && ( normkey_id > normpred)) {
            cout<<"Replaced first"<<vecfingerTable_[i].first<<" with first "<<node<<endl;
            cout<<"Replaced second"<<vecfingerTable_[i].second<<" with second "<<unsigned(key)<<endl;
            vecfingerTable_[i].first = node;
            vecfingerTable_[i].second = key;
        }
    }
}

/* @details     Copy fingertable in node id_'s prespective */
void
FingerTable::copyFingerTable(FingerTable srcft, uint8_t thisnodeid, uint8_t pred_id, uint8_t key)
{
    uint16_t normsuccessor = (successor_id < id_) ? successor_id + 255 : successor_id;
    for(uint8_t i=0; i<srcft.vecfingerTable_.size(); i++) {
        vecfingerTable_[i] = srcft.vecfingerTable_[i];

    }
}
