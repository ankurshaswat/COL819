#ifndef FINGER_H
#define FINGER_H

#include <string>
#include <node.h>

using namespace std;

class node;
class finger_table
{
public:
    finger_table(string);
    node *get_node(size_t);
    node *get_start(size_t);
    void set_node(size_t, node *);

private:
};

#endif