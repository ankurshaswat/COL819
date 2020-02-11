#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <node.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

node::node(string node_ip, string node_id, coordinator *coord)
{
    this->lf_set = new leaf_set(node_id);
    this->r_table = new routing_table(node_id);
    this->nb_set = new neighbourhood_set(node_id);
    this->node_id = node_id;
    this->node_ip = node_ip;
    this->coord = coord;
}

msg_type node::join_network(string neighbour_id)
{
    msg_type new_msg;

    new_msg.type = "join";
    new_msg.sender_id = this->node_id;
    new_msg.target_id = neighbour_id;
    new_msg.key = this->node_id;
    new_msg.value = "0";

    return new_msg;
}

vector<msg_type> node::receive_send_msg(msg_type msg)
{
    vector<msg_type> replies;

    DEBUG(this->node_id << " Received " << msg.type << " type msg from " << msg.sender_id << " Key " << msg.key);

    // Received a join type message.
    if (msg.type == "join")
    {
        // Need to route msg further.
        string route_target = route(msg.key);
        // DEBUG("Routing to " << route_target);
        if (route_target != "")
        {
            msg_type reply_msg;

            // Reply to original node with join_ack (equivalent to returning state tables)
            reply_msg.type = "join_ack";
            reply_msg.target_id = msg.key; // Replying to new node added
            reply_msg.sender_id = this->node_id;
            reply_msg.key = msg.key;
            reply_msg.value = msg.value; // State tables can be filled here. Instead doing with function calls.

            replies.push_back(reply_msg);

            msg_type route_msg;

            route_msg.type = "join";
            route_msg.target_id = route_target;
            route_msg.sender_id = this->node_id;
            route_msg.key = msg.key;
            route_msg.value = to_string(stoi(msg.value) + 1);

            replies.push_back(route_msg);
        }
        else
        {
            msg_type reply_msg;

            // Reply to original node with join_ack_final (equivalent to returning state tables)
            reply_msg.type = "join_ack_final";
            reply_msg.target_id = msg.key; // Replying to new node added
            reply_msg.sender_id = this->node_id;
            reply_msg.key = msg.key;
            reply_msg.value = msg.value; // Can be filled here. Instead doing with function calls.

            replies.push_back(reply_msg);
        }
        // TODO: Add node data to tables...
    }
    else if (msg.type == "join_ack")
    {
        // Add data from node to self.
        initialize_data(coord->get_node(msg.sender_id), stoi(msg.value));
        // DEBUG("node::receive_send_msg join_ack INCOMPLETE CASE");
    }
    else if (msg.type == "join_ack_final")
    {
        // Add data from this final node into self.
        initialize_data(coord->get_node(msg.sender_id), stoi(msg.value));

        // TODO: Inform all nodes in leaf set of arrival.
        this->lf_set->add_inform_msgs(replies);
        // TODO: Inform neighbour node to add in neighbour set ?
        // DEBUG("node::receive_send_msg " << msg.type << " INCOMPLETE CASE");
    }
    else if (msg.type == "inform")
    {
        // Add node to all state tables.
        this->r_table->insert_node(msg.sender_id);
        this->lf_set->insert_node(msg.sender_id);

        // DEBUG("node::receive_send_msg " << msg.type << " INCOMPLETE CASE");
    }
    else
    {
        DEBUG("node::receive_send_msg " << msg.type << " INCOMPLETE CASE");
    }

    return replies;
}

string node::route(string key)
{
    string next_node = "";

    if (this->lf_set->incomplete())
    {
        // Incomplete leaf set search all.
        next_node = this->lf_set->search_complete(key);
        DEBUG("Incomplete leafset " << next_node);
    }
    else if (this->lf_set->check_in_leaf_set(key))
    {
        // If target within range of leaf set
        // Find closest node in leaf set as next node.
        next_node = this->lf_set->find_closest_leaf(key);

        DEBUG("Closest Leaf in leafset " << next_node);
    }
    else
    {
        // Getting common prefix for checking in routing table.
        int len_common_prefix = get_common_prefix_len(key, this->node_id);

        // Get closest match from routing table according to common prefix
        string routing_match = this->r_table->get_routing_match(len_common_prefix, key);
        DEBUG("Routing match " << routing_match);

        if (routing_match != "")
        {
            next_node = routing_match;
        }
        else
        {
            string best_match = node_id;
            string min_dist = get_dist(key, best_match);
            int max_prefix_len = get_common_prefix_len(key, best_match);

            // Find closest nodes in all 3 data structures.
            // string leaf_set_closest = this->lf_set->find_closest_leaf(len_common_prefix, key);
            this->lf_set->search_complete(key, best_match, min_dist, max_prefix_len);
            DEBUG("Rare case - after leafset " << best_match);
            // string routing_table_closest = this->r_table->find_closest_node(len_common_prefix, key);
            this->r_table->search_complete(key, best_match, min_dist, max_prefix_len);
            DEBUG("Rare case - after routingTable " << best_match);
            // string nb_set_closest = this->nb_set->find_closest_node(len_common_prefix, key);
            this->nb_set->search_complete(key, best_match, min_dist, max_prefix_len);
            DEBUG("Rare case - after nbSet " << best_match);

            // vector<string> possibile_next_nodes{leaf_set_closest, routing_table_closest, nb_set_closest};

            // Select the overall closest node as next node
            // next_node = find_closest_node_util(key, possibile_next_nodes);
            next_node = best_match;
        }
    }

    if (next_node == node_id)
    {
        next_node = "";
    }

    cout << "Routing Message to \"" << next_node << "\"" << endl;
    return next_node;
}

void node::initialize_data(node *old_node, int hop_num)
{
    lf_set->initialize_leaf_set(old_node->lf_set);
    r_table->initialize_routing_table(old_node->r_table);
}