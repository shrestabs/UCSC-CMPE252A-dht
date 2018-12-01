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
/* write function joinnode for 3 steps */
int main(int argc, char *argv[])

{
    uint8_t temp = 63;
    temp = temp - 72;
    cout<<"dfsdfs "<<unsigned(temp)<<endl;
    uint8_t sds = 8;
    for(int i=0;i<30; i++) {
        
        sds = sds -1 ;
        cout<<unsigned(sds)<<"\t";
    }
    cout<<endl;
    /*
     * TODO: For now, keys are integers ranging 0-255 for simplicity.
     * convert them to key = trunc(SHA1(ip + port)) 
     * keys are id and sent to constructor directly
     */
    Node n0(5);         /* node_id == 5 */
    Node n1(63);        /* node_id == 63 */
    Node n2(120);
    cout<<setfill ('x')<<setw(80)<<"TEST1xxxxxxxxxjoinxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.join(NULL);      /* the first node to join the Chord network. */
    cout<<setw(80)<<"TEST2xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(3, 3);    /* insert key = 3 */

    //Manually suc of new node prep -> not required for 2 nodes
    cout<<setw(80)<<"TEST4xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(5, 5);       /* insert key = 5 */
    cout<<setw(80)<<"TEST5xxxxxxxxxfindxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    /* key query. sequence of nodes to talk to */
    bool res = n0.find(5);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"TEST5 element not found"<<endl;
    cout<<setw(80)<<"TEST6xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(71, 71);       /* insert key = 5 */
    cout<<setw(80)<<"TEST7xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(2, 2);       /* insert key = 5 */
    cout<<setw(80)<<"TEST8xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(9, 9);       /* insert key = 5 */


    cout<<setw(80)<<"TEST3xxxxxxxxxjoinxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.join(&n0);       /* the second node join the Chord network. */

    res = n0.find(9);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"TEST5 element not found"<<endl;

    n2.join(&n0);
    /* event loop */
    
    return 0;
}