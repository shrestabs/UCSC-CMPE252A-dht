/**
 * @file           chord.cpp
 * @brief          Entry point to the chord implementation
 *
 * Runs the test cases for chord protocol
 * 
 * @author         Shresta B.S
 * @date           23/Oct/2018
 * @version         0.1   Initial version
 *                  1.0   Fingertable prettyPrint and nodes in the ring 
 *                  2.0   Working find, join, insert
 *                  Future versions
 *                  3.0   Implement Node leave
 *                  4.0   Implement SHA for key
 *                  5.0   Extend key, M and value range
 */

#include <iostream>
#include <iomanip>
#include <openssl/sha.h>

#include "node.h"

using namespace std;

/** 
 * @brief       Entry point to the project. 
 * 
 * Consists of test cases to test the project. Will be moved by unit-test 
 * module with event loop model
 * 
 * @return      0 on success
 */
int main(int argc, char *argv[])
{
    /**
     * TODO: Enchancement1 - change numeric keys to SHA hashes
     */
    Node n0(5);             /* node_id == 5 */
    Node n1(63);            /* node_id == 63 */
    Node n2(120);           /* node_id == 120 */
    Node n3(41);            /* node_id == 41 */
    Node n4(83);            /* node_id == 83 */
    Node n5(253);           /* node_id == 253 */
    Node n6(154);           /* node_id == 154 */

    cout<<setfill ('x')<<setw(80)<<"TEST1xxxxxxxxxjoinxxxxxxxxxxxxxxxxxx"<<endl;
    n0.join(NULL);          /* the first node to join the Chord network. */

    cout<<setw(80)<<"TEST2xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(3, 3);        /* insert key = 3 */

    cout<<setw(80)<<"TEST3xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(5, 5);        /* insert key = 5 */

    cout<<setw(80)<<"TEST4xxxxxxxxxfindxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    bool res = n0.find(5);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setw(80)<<"TEST5xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(71, 71);       /* insert key = 71 */

    cout<<setw(80)<<"TEST6xxxxxxxxxjoinxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.join(&n0);       /* the second node join the Chord network. */

    cout<<setw(80)<<"TEST7xxxxxxxxxfindxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    res = n0.find(9);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST8xxxxxxxxxjoinxxxxxxxxxxxxxxxxxx"<<endl;
    n2.join(&n0);

    cout<<setw(80)<<"TEST9xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;   
    n0.insert(2, 2);       /* insert key = 2 */

    cout<<setfill ('x')<<setw(80)<<"TEST10xxxxxxxxxjoinxxxxxxxxxxxxxxxxx"<<endl;
    n3.join(&n1);

    cout<<setw(80)<<"TEST11xxxxxxxxxinsertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(9, 9);       /* insert key = 5 */

    cout<<setfill ('x')<<setw(80)<<"TEST12xxxxxxxxxjoinxxxxxxxxxxxxxxxxx"<<endl;
    n4.join(&n0);

    cout<<setfill ('x')<<setw(80)<<"TEST13xxxxxxxxxjoinxxxxxxxxxxxxxxxxx"<<endl;
    n5.join(&n2);

    cout<<setfill ('x')<<setw(80)<<"TEST14xxxxxxxxxfindxxxxxxxxxxxxxxxxx"<<endl;
    res = n5.find(71);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST15xxxxxxxxxfindxxxxxxxxxxxxxxxxxx"<<endl;
    res = n5.find(84);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    return 0;
}