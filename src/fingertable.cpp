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


/* @details     Copy fingertable in node id_'s prespective 
void
FingerTable::copyFingerTable(uint8_t thisnodeid, uint8_t pred_id, uint8_t key, Node* pred)
{
    uint16_t normsuccessor = (successor_id < id_) ? successor_id + 255 : successor_id;
    for(uint8_t i=0; i<srcft.vecfingerTable_.size(); i++) {
        vecfingerTable_[i] = pred->re remoteRecursiveLookup CHORDJUMP(thisnodeid, i)

    }
}
*/