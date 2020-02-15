#ifndef NODE_H
#define NODE_H

#include <string>
#include <finger_table.h>
#include <utils.h>
#include <vector>

using namespace std;

class finger_table;
class node
{
public:
    node(string, size_t);
    void join(node *);
    void init_finger_table(node *);
    void update_others();
    void update_finger_table(node *, size_t);
    node *find_successor(size_t);
    node *find_predecessor(size_t);
    // string get_finger_start(int);
    bool operator<(const node &);
    bool operator==(const node &);
    bool operator<=(const node &);
    bool operator>(const node &);
    bool operator>=(const node &);
    node *closest_preceding_finger(size_t id);
    // vector<msg_type> receive_send_msg(msg_type);

private:
    finger_table *ftable;
    node *successor, *predecessor;
    size_t node_id;
    string node_ip;
};

#endif