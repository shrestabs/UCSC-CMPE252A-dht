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

void
Node::join(Node* node)
{


}

uint8_t
Node::find(uint8_t key)
{
    /* print sequence of nodes to talk to */
    cout<<key<<" not found"<<endl;
    return 0;
}

void
Node::remove(uint8_t key)
{
    cout<<"Cannot exit the network. Feature not implemented :) "<<endl;
}

void
Node::insert(uint8_t key, uint8_t value)
{
    /* key to insert. in the real work key,value can be key,music.mp3 or key,book.pdf */
    /* Move keys to */
    cout<<key<<" not inserted"<<endl;
}