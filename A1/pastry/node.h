#ifndef NODE_H
#define NODE_H

#include <leaf_set.h>
#include <routing_table.h>
#include <neighbourhood_set.h>
#include <utils.h>
#include <coordinator.h>

class coordinator;
class node
{
public:
    node(string, string, coordinator *);
    vector<msg_type> receive_send_msg(msg_type);
    msg_type join_network(string);
    void initialize_data(node *, int);

private:
    string route(string);
    void add_node_data(node *);

    leaf_set *lf_set;
    routing_table *r_table;
    neighbourhood_set *nb_set;
    string node_ip, node_id;
    coordinator *coord;
};

#endif
