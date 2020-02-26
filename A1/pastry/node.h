#ifndef NODE_H
#define NODE_H

#include <leaf_set.h>
#include <routing_table.h>
#include <neighbourhood_set.h>
#include <utils.h>
#include <coordinator.h>
#include <unordered_map>

class coordinator;
class node
{
public:
    node(string node_ip, string node_id, coordinator *coord);
    ~node();

    vector<msg_type> receive_send_msg(msg_type);
    msg_type join_network(string);
    void initialize_data(node *);
    string get(string key);
    string get_identifier(string key_identifier, int num_hops);
    void put(string key, string val);
    void put_identifier(string key_identifier, string val);
    void delete_self();
    void notify_delete(string node_id);
    int get_num_keys();
    void print_routing_table();

private:
    string route(string);
    void add_node_data(node *);
    string retreive_data(string key_identifier);
    void store_data(string key_identifier, string val);
    // void remove_deleted_node(string node_id);

    size_t num_routes;
    leaf_set *lf_set;
    routing_table *r_table;
    neighbourhood_set *nb_set;
    string node_ip, node_id;
    coordinator *coord;
    unordered_map<string, string> data_store;
    bool enable_logs;
};

#endif
