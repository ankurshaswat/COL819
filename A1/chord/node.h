#ifndef NODE_H
#define NODE_H

#include <string>
#include <finger_table.h>

using namespace std;

class finger_table;
class node
{
public:
    node(string, string);
    void join(node *);
    void init_finger_table(node *);
    void update_others();
    void update_finger_table(node *,size_t);
    node* find_successor(node *);
    node* find_predecessor(node *);
    node *find_predecessor(size_t);
    // string get_finger_start(int);
    bool operator<(const node &);
    bool operator==(const node &);
    bool operator<=(const node &);
    bool operator>(const node &);
    bool operator>=(const node &);

private:
    finger_table *ftable;
    node *successor, *predecessor;
    size_t node_id;
};

#endif