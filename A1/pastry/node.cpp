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

node::node(string node_ip, string node_id, coordinator *coord, bool enable_logs) : num_routes(0), node_ip(node_ip), node_id(node_id), coord(coord), enable_logs(enable_logs)
{
    this->lf_set = new leaf_set(node_id);
    this->r_table = new routing_table(node_id);
    this->nb_set = new neighbourhood_set(node_id);

    if (enable_logs)
    {
        cout << "|----------------------|" << endl;
        cout << "|S.No.\t|Target\t|Successor\t|" << endl;
        cout << "|----------------------|" << endl;
    }
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

    //* Received a join type message.
    if (msg.type == "join")
    {
        //* Need to route msg further.
        string route_target = route(msg.key);
        if (route_target != "")
        {
            msg_type reply_msg;

            //* Reply to original node with join_ack (equivalent to returning state tables)
            reply_msg.type = "join_ack";
            reply_msg.target_id = msg.key; //* Replying to new node added
            reply_msg.sender_id = this->node_id;
            reply_msg.key = msg.key;
            reply_msg.value = msg.value; //* State tables can be filled here. Instead doing with function calls.

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

            //* Reply to original node with join_ack_final (equivalent to returning state tables)
            reply_msg.type = "join_ack_final";
            reply_msg.target_id = msg.key; //* Replying to new node added
            reply_msg.sender_id = this->node_id;
            reply_msg.key = msg.key;
            reply_msg.value = msg.value; //* State tables can be filled here. Instead doing with function calls.

            replies.push_back(reply_msg);
        }
    }
    else if (msg.type == "join_ack")
    {
        //* Add data from node to self.
        initialize_data(coord->get_node(msg.sender_id));
    }
    else if (msg.type == "join_ack_final")
    {
        //* Add data from this final node into self.
        initialize_data(coord->get_node(msg.sender_id));

        vector<string> leaves;
        vector<string>::iterator itr;

        // //* Query all leaves and collect tables.
        // leaves = lf_set->get_leaves();
        // for (itr = leaves.begin(); itr != leaves.end(); ++itr)
        // {
        //     initialize_data(coord->get_node(*itr));
        // }

        //* Inform all nodes in leaf set of arrival.
        leaves = lf_set->get_leaves();
        for (itr = leaves.begin(); itr != leaves.end(); ++itr)
        {
            msg_type inform_msg;

            inform_msg.type = "inform";
            inform_msg.sender_id = node_id;
            inform_msg.target_id = *itr;
            inform_msg.key = *itr;
            inform_msg.value = "";

            replies.push_back(inform_msg);
        }
        //?: Inform neighbour node to add in neighbour set ?
    }
    else if (msg.type == "inform")
    {
        //* Add node to all state tables.
        this->r_table->insert_node(msg.sender_id);
        this->lf_set->insert_node(msg.sender_id);
    }
    else
    {
        DEBUG("node::receive_send_msg " << msg.type << " INCOMPLETE CASE");
    }

    return replies;
}

string node::route(string key)
{
    string next_node;

    while (true)
    {

        next_node = "";

        if (this->lf_set->incomplete())
        {
            //* Incomplete leaf set search all.

            // next_node = this->lf_set->search_incomplete_set(key);
            next_node = this->lf_set->search_complete(key);
            DEBUG("Incomplete leafset " << next_node);
        }
        else if (this->lf_set->check_in_leaf_set(key))
        {
            //* If target within range of leaf set find closest node in leaf set as next node.

            next_node = this->lf_set->find_closest_leaf(key);
            DEBUG("Closest Leaf in leafset " << next_node);
        }
        else
        {
            // Getting common prefix for checking in routing table.
            int len_common_prefix = get_common_prefix_len(key, this->node_id);

            // Get closest match from routing table according to common prefix
            string routing_match = this->r_table->get_routing_match(len_common_prefix, key);
            DEBUG("Routing Table entry " << routing_match);

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

        if ((next_node == node_id) || (next_node == "") || coord->check_exist(next_node))
        {
            break;
        }
        else
        {
            notify_delete(next_node);
        }
    }

    if (next_node == node_id)
    {
        next_node = "";
        if (enable_logs)
            cout << "|  " << ++num_routes << "\t  | " << key << "\t   | " << node_id << "(Self)\t  |  " << endl;
    }
    else if (enable_logs)
        cout << "|  " << ++num_routes << "\t  | " << key << "\t   | " << next_node << "\t  |  " << endl;

    DEBUG("Routing Message to \"" << next_node << "\"");
    return next_node;
}

void node::initialize_data(node *old_node)
{
    // set<string> all_nodes = old_node->get_nodes();
    lf_set->initialize_leaf_set(old_node->lf_set);
    r_table->initialize_routing_table(old_node->r_table);

    //? Add entries of lf set to rtable
    //? Add entries of rtable to lfset
}

string node::get(string key)
{
    string key_identifier = upper_md5(key);
    return get_identifier(key_identifier, 0);
}

string node::get_identifier(string key_identifier, int num_hops)
{
    string next_node_id = route(key_identifier);
    if (next_node_id == this->node_id || next_node_id == "")
    {
        coord->write_hop_num(num_hops);
        return retreive_data(key_identifier);
    }
    else
    {
        node *next_node = coord->get_node(next_node_id);
        return next_node->get_identifier(key_identifier, num_hops + 1);
    }
}

void node::put(string key, string val)
{
    string key_identifier = upper_md5(key);
    put_identifier(key_identifier, val);
}

void node::put_identifier(string key_identifier, string val)
{
    string next_node_id = route(key_identifier);
    if (next_node_id == this->node_id || next_node_id == "")
    {
        return store_data(key_identifier, val);
    }
    else
    {
        node *next_node = coord->get_node(next_node_id);
        return next_node->put_identifier(key_identifier, val);
    }
}

void node::delete_self()
{
    //* Inform all nodes of deletion
    //* Routing table, leaf set, neighbourhood set
    vector<string> node_ids = r_table->get_nodes();
    for (size_t i = 0; i < node_ids.size(); i++)
    {
        if (coord->check_exist(node_ids[i]))
        {
            coord->get_node(node_ids[i])->notify_delete(node_id);
        }
    }

    node_ids = lf_set->get_leaves();
    for (size_t i = 0; i < node_ids.size(); i++)
    {
        if (coord->check_exist(node_ids[i]))
        {
            coord->get_node(node_ids[i])->notify_delete(node_id);
        }
    }

    node_ids = nb_set->get_neighbours();
    for (size_t i = 0; i < node_ids.size(); i++)
    {
        if (coord->check_exist(node_ids[i]))
        {
            coord->get_node(node_ids[i])->notify_delete(node_id);
        }
    }
}

void node::notify_delete(string node_id)
{
    // Remove from leaf set
    string get_leaf_set = lf_set->remove(node_id);
    // Fix leaf set if possible
    if (get_leaf_set != "")
    {
        node *to_fetch_node_states = coord->get_node(get_leaf_set);
        leaf_set *fetched_leaf_set = to_fetch_node_states->lf_set;
        vector<string> leaves = fetched_leaf_set->get_leaves();
        for (size_t i = 0; i < leaves.size(); i++)
        {
            if (leaves[i] == node_id || leaves[i] == this->node_id)
            {
                continue;
            }
            lf_set->insert_node(leaves[i]);
        }
    }

    // Remove from routing table
    bool found_and_removed = r_table->remove(node_id);
    // Try to fix routing table
    if (found_and_removed)
    {
        size_t len_common_prefix = get_common_prefix_len(node_id, this->node_id);
        char next_char = node_id[len_common_prefix];
        int next_char_int = hex_to_int(next_char);
        // Now need to fix by querying other rtables;

        bool fixed = false;
        for (size_t i = len_common_prefix; i < r_table->get_size(); i++)
        {
            vector<string> fixer_ids = r_table->get_possible_fixers(i, next_char_int);
            for (size_t j = 0; j < fixer_ids.size(); j++)
            {
                if (!coord->check_exist(fixer_ids[j]))
                {
                    continue;
                }
                routing_table *new_r_table = coord->get_node(fixer_ids[j])->r_table;
                string new_node_id = r_table->get_routing_match(len_common_prefix, node_id);
                if (new_node_id == "")
                {
                    continue;
                }
                else if (!coord->check_exist(new_node_id))
                {
                    continue;
                }
                else
                {
                    r_table->insert_node(new_node_id);
                    fixed = true;
                    break;
                }
            }
            if (fixed)
            {
                break;
            }
        }
    }

    // Remove from neighbour set.
    nb_set->remove(node_id);
}

string node::retreive_data(string key_identifier)
{
    if (data_store.find(key_identifier) == data_store.end())
    {
        return "NOT FOUND";
    }
    else
    {
        return data_store[key_identifier];
    }
}

void node::store_data(string key_identifier, string val)
{
    data_store[key_identifier] = val;
}

node::~node()
{
    delete lf_set;
    delete r_table;
    delete nb_set;
}

int node::get_num_keys()
{
    return data_store.size();
}