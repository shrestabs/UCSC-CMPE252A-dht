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
    cout<<"Constructor of fingertable"<<endl;
    fingerTableSize = BITLENGTH;
    /* According to Chord paper, the finger table starts from index=1 */
    for(uint8_t i=0; i<BITLENGTH ; i++) {
        vecfingerTable_.push_back(NULL);
    }
    /* why not use a circular buffer from boost library */
}


void
FingerTable::prettyPrint()
{
    cout<<"Finger table print of "<<unsigned(nodeId_)<<endl;
    for(uint8_t i=0; i<vecfingerTable_.size(); i++) {
        //FIXME: make it pretty
        if(vecfingerTable_[i] == NULL) {
            cout<<"NULL"<<"\t";
        }
        else {
            //cout<<(vecfingerTable_[i])->nodeId_<<"\t";
            cout<<"NOTNULL"<<"\t";
        }
    }
    cout<<endl;
}

void
FingerTable::copyFingerTable(FingerTable srcft)
{
    for(uint8_t i=0; i<srcft.vecfingerTable_.size(); i++) {
        vecfingerTable_[i] = srcft.vecfingerTable_[i];
    }
}
