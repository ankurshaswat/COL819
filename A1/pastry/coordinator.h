#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <queue>
#include <unordered_map>
#include <node.h>

class node;
class coordinator
{
public:
    coordinator(int);
    void start_simulation();
    node* get_node(string);

private:
    void insert_node_data(string, string, node *);
    string sample_close_neighbour(string);
    void handle_msgs();
    bool check_free_ip(string);
    bool check_free_node_id(string);
    int num_nodes;
    vector<string> ip_list;
    queue<msg_type> msg_q;
    unordered_map<string, node *> node_map;
    unordered_map<string, bool> node_ip_map, node_id_map;
};

#endif
