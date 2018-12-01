/**
 * @file           fingertable.cpp
 * @brief          Implements the node's fingertable.
 * 
 * Initialized the fingertable for a new node. Prints the ve

@author         Shresta B.S
@date           23/Oct/2018
*/
#include "fingertable.h"

/**
    @fn         FingerTable
    @brief      Constructor of fingertable
    @details    Log client application MUST call this API to uninitialize
    @param      Reference to timer that needs to be started
    @return     int (errno)
*/
FingerTable::FingerTable(uint8_t nodeId): nodeId_(nodeId)
{
    Node *dummy = NULL;
    DEBUG("-----Constructor of fingertable----"<<endl);
    fingerTableSize = BITLENGTH;
    /* According to Chord paper, the finger table starts from index=1 */
    for(uint8_t i=0; i<BITLENGTH ; i++) {
        vecfingerTable_.push_back(std::make_pair(dummy,i));
    }
}

/**
    @fn         prettyPrint
    @brief      Prints fingertable of the node that stores this table

    @details    Prints pairs of the fingertable
                E.g.Address:0x7ffee2c380a0  successor value:5
    @param[in]  none
    @return     none
*/
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
        cout<<"successor value:"<<unsigned(vecfingerTable_[i].second)<<"\n";
    }
    cout<<endl;
}