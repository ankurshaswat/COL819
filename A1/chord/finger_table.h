#ifndef FINGER_H
#define FINGER_H

#include <string>
#include <node.h>
#include <unordered_map>

using namespace std;

class node;
class finger_table
{
public:
    node *get_node(size_t);
    node *get_start(size_t);
    void set_node(size_t, node *);

private:
    unordered_map<size_t, node *> starts;
    unordered_map<size_t, node *> nodes;
};

#endif