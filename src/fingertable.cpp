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
    /* According to Chord paper, the finger table starts from index=1 */
    fingerTable_.resize(BITLENGTH + 1);
}

void FingerTable::prettyPrint()
{

}

void FingerTable::stablizeNode()
{

}