/*
@file           chord.cpp
@brief          Entry point to the chord implementation

@details        Runs the event loop in 
@author         Shresta B.S
@date           23/Oct/2018
@version        0.1   Initial version
                1.0   Working find, join
                2.0   Implement Node leave
                3.0   Implement SHA for key
                4.0   Extend key, M and value range
*/

#include <iostream>
#include <openssl/sha.h>

#include "node.h"

using namespace std;

int main(int argc, char *argv[])

{
    /*
     * TODO: For now, keys are integers ranging 0-127 for simplicity.
     * convert them to key = trunc(SHA1(ip + port)) 
     */

    Node n0(5);         /* node_id == 5 */
    Node n1(63);        /* node_id == 63 */
    n0.join(NULL);      /* the first node to join the Chord network. */
    n0.insert(3, 3);    /* insert key = 3 */
    n1.join(&n0);       /* the second node join the Chord network. */
    n1.insert(5, 5);       /* insert key = 5 */
    n0.find(5);         /* key query. sequence of nodes to talk to */

    /* event loop */
    return 0;
}