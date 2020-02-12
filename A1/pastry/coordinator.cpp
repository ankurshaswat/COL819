#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <coordinator.h>
#include <node.h>
#include <utils.h>
#include <queue>
#include <algorithm>
#include <iostream>

coordinator::coordinator(int num_nodes)
{
    DEBUG("Num nodes in simulation = " << num_nodes);
    this->num_nodes = num_nodes;
}

node *coordinator::get_node(string node_id)
{
    return node_map[node_id];
}

void coordinator::start_simulation()
{

    for (int i = 0; i < this->num_nodes; i++)
    {
        string new_ip, node_id;
        while (true)
        {
            new_ip = get_random_ip();
            node_id = upper_md5(new_ip);
            if (!this->check_free_ip(new_ip) || !this->check_free_node_id(node_id))
            {
                DEBUG("Continuing");
                continue;
            }
            else
            {
                break;
            }
        }
        node *next_node = new node(new_ip, node_id, this);

        cout << "Node:\"" << i << "\" NodeID:\"" << node_id << "\" IP:\"" << new_ip << "\"" << endl;

        string starting_neighbour = this->sample_close_neighbour(new_ip);

        this->insert_node_data(new_ip, node_id, next_node);

        if (starting_neighbour != "")
        {
            msg_type joining_msg = next_node->join_network(starting_neighbour);
            this->msg_q.push(joining_msg);
        }

        this->handle_msgs();
    }
}

void coordinator::handle_msgs()
{
    while (this->msg_q.size() > 0)
    {
        msg_type next_msg = this->msg_q.front();
        this->msg_q.pop();

        // get node to send to in next_msg
        node *receiver_node = this->node_map[next_msg.target_id];
        vector<msg_type> replies = receiver_node->receive_send_msg(next_msg);
        for (size_t i = 0; i < replies.size(); i++)
        {
            this->msg_q.push(replies[i]);
        }
    }
}

bool coordinator::check_free_ip(string ip)
{
    // DEBUG(this->node_ip_map[ip]);
    return !this->node_ip_map[ip];
}

bool coordinator::check_free_node_id(string node_id)
{
    // DEBUG(this->node_id_map[node_id]);
    return !this->node_id_map[node_id];
}

void coordinator::insert_node_data(string ip, string node_id, node *new_node)
{
    this->node_ip_map[ip] = true;
    this->ip_list.push_back(ip);

    this->node_id_map[node_id] = true;

    this->node_map[node_id] = new_node;
}

string coordinator::sample_close_neighbour(string new_ip)
{
    if (this->ip_list.size() == 0)
    {
        return "";
    }

    size_t pos = lower_bound(this->ip_list.begin(), this->ip_list.end(), new_ip) - this->ip_list.begin();
    if (pos >= this->ip_list.size())
    {
        pos--;
    }
    // DEBUG("Neighbour pos = " << pos << " and neighbour = " << this->ip_list[pos]);
    return upper_md5(this->ip_list[pos]);
}