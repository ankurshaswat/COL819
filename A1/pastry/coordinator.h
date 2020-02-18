#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <queue>
#include <unordered_map>
#include <node.h>

class node;
class coordinator
{
public:
    coordinator(int, int, int, int, bool, int);
    void start_simulation();
    node *get_node(string);
    bool check_exist(string node_id);

private:
    void insert_node_data(string, string, node *);
    void remove_node_data(string ip, string id);
    string sample_close_neighbour(string);
    void handle_msgs();
    bool check_free_ip(string);
    bool check_free_node_id(string);
    vector<string> ip_list, node_list, data;
    queue<msg_type> msg_q;
    unordered_map<string, node *> node_map;
    unordered_map<string, bool> node_ip_map, node_id_map;

    size_t num_nodes, num_data_elements, num_search_queries, num_node_add_queries, num_node_delete_queries, num_data_add_queries;
    bool enable_logs;
    size_t log_node;
};

#endif
