#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <queue>
#include <unordered_map>
#include <node.h>
#include <utils.h>

using namespace std;

class node;
class coordinator
{
public:
    coordinator(int);
    void start_simulation();
    node* get_node(size_t);

private:
    void insert_node_data(string, size_t, node *);
    size_t sample_node();
    void handle_msgs();
    bool check_free_ip(string);
    bool check_free_node_id(size_t);
    int num_nodes;
    vector<size_t> node_list;
    queue<msg_type> msg_q;
    unordered_map<size_t, node *> node_map;
    unordered_map<string, bool> node_ip_map;
    unordered_map<size_t, bool> node_id_map;
};

#endif
