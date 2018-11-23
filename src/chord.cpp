/*
@file           chord.cpp
@brief          Entry point to the chord implementation

@details        Runs the event loop in 
@author         Shresta B.S
@date           23/Oct/2018
@version        0.1   Initial version
                1.0   Fingertable prettyPrint and nodes in the ring 
                2.0   Working find, join
                3.0   Implement Node leave
                4.0   Implement SHA for key
                5.0   Extend key, M and value range
*/

#include <iostream>
#include <iomanip>
#include <openssl/sha.h>

#include "node.h"

using namespace std;

int main(int argc, char *argv[])

{
    /*
     * TODO: For now, keys are integers ranging 0-255 for simplicity.
     * convert them to key = trunc(SHA1(ip + port)) 
     * keys are id and sent to constructor directly
     */
    Node n0(5);         /* node_id == 5 */
    Node n1(63);        /* node_id == 63 */
    cout<<setfill ('x')<<setw(80)<<"TEST1xxxxxxxxxjoinxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.join(NULL);      /* the first node to join the Chord network. */
    //FIXME: if output is expected print PASS
    cout<<setw(80)<<"TEST2xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(3, 3);    /* insert key = 3 */
    cout<<setw(80)<<"TEST3xxxxxxxxxjoinxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.join(&n0);       /* the second node join the Chord network. */
    
    cout<<setw(80)<<"TEST4xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.insert(5, 5);       /* insert key = 5 */
    cout<<setw(80)<<"TEST5xxxxxxxxxfindxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    /* key query. sequence of nodes to talk to */
    int res = n0.find(5);
    if(res == -1)
        cout<<"TEST5 element not found"<<endl;
    else
        cout<<"element "<<res<< " found"<<endl;

    /* event loop */
    return 0;
}