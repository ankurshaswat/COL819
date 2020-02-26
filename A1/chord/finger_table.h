#ifndef FINGER_H
#define FINGER_H

#include <string>
#include <node.h>
#include <unordered_map>
#include <vector>

using namespace std;

class node;
class finger_table
{
public:
    finger_table(size_t node_id);
    size_t get_node(size_t);
    size_t get_start(size_t);
    void set_node(size_t, size_t);
    void print_finger_table();
    
private:
    vector<size_t> starts, nodes;
    // unordered_map<size_t, size_t> starts;
    // unordered_map<size_t, node *> nodes;
    size_t node_id;
};

#endif