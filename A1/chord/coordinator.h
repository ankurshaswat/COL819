#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <queue>
#include <unordered_map>
#include <node.h>
#include <utils.h>
#include <fstream>

using namespace std;

class node;
class coordinator
{
public:
    coordinator(int num_search_queries, int num_node_add_queries, int num_node_delete_queries, int num_data_add_queries, bool enable_logs, int log_node, string log_name);
    void start_simulation();
    node *get_node(size_t);
    bool check_exist(size_t node_id);
    ~coordinator();
    void write_hop_num(int num_hops);

private:
    void insert_node_data(string, size_t, node *);
    void remove_node_data(string ip, size_t id);
    size_t sample_node();
    bool check_free_ip(string);
    bool check_free_node_id(size_t);

    vector<size_t> node_list;
    vector<string> ip_list, data;
    unordered_map<size_t, node *> node_map;
    unordered_map<string, bool> node_ip_map;
    unordered_map<size_t, bool> node_id_map;
    size_t num_nodes = 0, num_data_elements, num_search_queries, num_node_add_queries, num_node_delete_queries, num_data_add_queries, log_node;
    bool enable_logs;
    ofstream log_file;
    string log_name;
};

#endif
