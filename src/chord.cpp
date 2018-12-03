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

    cout<<setfill ('x')<<setw(80)<<"TEST1xxxxxxxxxjoin 5xxxxxxxxxxxxxxxx"<<endl;
    n0.join(NULL);          /* the first node to join the Chord network. */

    cout<<setw(80)<<"TEST2xxxxxxxxxinsert 3xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(3, 3);        /* insert key = 3 */

    cout<<setw(80)<<"TEST3xxxxxxxxxinsert 5xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(5, 5);        /* insert key = 5 */

    cout<<setw(80)<<"TEST4xxxxxxxxxfind 5xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    bool res = n0.find(5);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setw(80)<<"TEST5xxxxxxxxxinsert 71xxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(71, 71);       /* insert key = 71 */

    cout<<setw(80)<<"TEST6xxxxxxxxxjoin 63xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.join(&n0);       /* the second node join the Chord network. */

    cout<<setw(80)<<"TEST7xxxxxxxxxfind 9xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    res = n0.find(9);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST8xxxxxxxxxjoin 120xxxxxxxxxxxxxx"<<endl;
    n2.join(&n0);

    cout<<setw(80)<<"TEST9xxxxxxxxxinsert 2xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;   
    n0.insert(2, 2);       /* insert key = 2 */

    cout<<setfill ('x')<<setw(80)<<"TEST10 41xxxxxxxxxjoinxxxxxxxxxxxxxx"<<endl;
    n3.join(&n1);

    cout<<setw(80)<<"TEST11xxxxxxxxxinsert 9xxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(9, 9);       /* insert key = 5 */

    cout<<setfill ('x')<<setw(80)<<"TEST12 83xxxxxxxxxjoinxxxxxxxxxxxxxx"<<endl;
    n4.join(&n0);

    cout<<setfill ('x')<<setw(80)<<"TEST13xxxxxxxxxjoin 253xxxxxxxxxxxxx"<<endl;
    n5.join(&n2);

    cout<<setfill ('x')<<setw(80)<<"TEST14xxxxxxxxxfind 71xxxxxxxxxxxxxx"<<endl;
    res = n5.find(71);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST15xxxxxxxxxfind 84xxxxxxxxxxxxxx"<<endl;
    res = n5.find(84);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;
    cout<<setw(80)<<"TEST16xxxxxxxxxinsert 132xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(132, 132);       /* insert key = 132 */

    cout<<setw(80)<<"TEST17xxxxxxxxxinsert 91xxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.insert(91, 91);       /* insert key = 91 */
    
    cout<<setw(80)<<"TEST18xxxxxxxxxinsert 200xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n2.insert(200, 200);       /* insert key = 200 */
    
    cout<<setw(80)<<"TEST19xxxxxxxxxinsert 27xxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(27, 27);       /* insert key = 27 */
    
    cout<<setw(80)<<"TEST20xxxxxxxxxinsert 145xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.insert(145, 145);       /* insert key = 145 */
    
    cout<<setw(80)<<"TEST21xxxxxxxxxinsert 129xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n3.insert(129, 129);       /* insert key = 129 */
    
    cout<<setw(80)<<"TEST22xxxxxxxxxinsert 53xxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n4.insert(53, 53);       /* insert key = 53 */
    
    cout<<setw(80)<<"TEST23xxxxxxxxxinsert 212xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n5.insert(212, 212);       /* insert key = 212 */
    
    cout<<setw(80)<<"TEST24xxxxxxxxxinsert 148xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n6.insert(148, 148);       /* insert key = 148 */
    
    cout<<setw(80)<<"TEST25xxxxxxxxxinsert 12xxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n0.insert(12, 12);       /* insert key = 12 */
    
    cout<<setw(80)<<"TEST26xxxxxxxxxinsert 124xxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    n1.insert(124, 124);       /* insert key = 124 */

    cout<<setfill ('x')<<setw(80)<<"TEST27xxxxxxxxxfind 132xxxxxxxxxxxxx"<<endl;
     res = n5.find(132);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST28xxxxxxxxxfind 212xxxxxxxxxxxxx"<<endl;
    res = n3.find(212);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST29xxxxxxxxxfind 53xxxxxxxxxxxxxx"<<endl;
    res = n2.find(53);
    if(res)
        cout<<"element found"<<endl;
    else
        cout<<"element not found"<<endl;

    cout<<setfill ('x')<<setw(80)<<"TEST30xxxxxxxxxjoin 154xxxxxxxxxxxxx"<<endl;
    n6.join(&n2);

    return 0;
}