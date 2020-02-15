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
#include <iostream>

using namespace std;

coordinator::coordinator(int num_nodes)
{
    DEBUG("Num nodes in simulation = " << num_nodes);
    this->num_nodes = num_nodes;
}

node *coordinator::get_node(size_t node_id)
{
    return node_map[node_id];
}

void coordinator::handle_msgs()
{
    // while (this->msg_q.size() > 0)
    // {
    //     msg_type next_msg = this->msg_q.front();
    //     this->msg_q.pop();

    //     // get node to send to in next_msg
    //     node *receiver_node = this->node_map[next_msg.target_id];
    //     vector<msg_type> replies = receiver_node->receive_send_msg(next_msg);
    //     for (size_t i = 0; i < replies.size(); i++)
    //     {
    //         this->msg_q.push(replies[i]);
    //     }
    // }
}

bool coordinator::check_free_ip(string ip)
{
    // DEBUG(this->node_ip_map[ip]);
    return !this->node_ip_map[ip];
}

bool coordinator::check_free_node_id(size_t node_id)
{
    // DEBUG(this->node_id_map[node_id]);
    return !this->node_id_map[node_id];
}

void coordinator::insert_node_data(string ip, size_t node_id, node *new_node)
{
    this->node_ip_map[ip] = true;
    this->node_list.push_back(node_id);
    // this->ip_list.push_back(ip);

    this->node_id_map[node_id] = true;

    this->node_map[node_id] = new_node;
}

size_t coordinator::sample_node()
{
    size_t random_node = rand() % node_list.size() + 1;
    return node_list[random_node];
}

void coordinator::start_simulation()
{

    for (int i = 0; i < this->num_nodes; i++)
    {
        string new_ip;
        size_t node_id;

        while (true)
        {
            new_ip = get_random_ip();
            node_id = hash_custom(new_ip);
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
        node *next_node = new node(new_ip, node_id);

        cout << "Node:\"" << i << "\" NodeID:\"" << node_id << "\" IP:\"" << new_ip << "\"" << endl;

        if (node_list.size() == 0)
        {
            next_node->join(NULL);
        }
        else
        {
            size_t starting_neighbour = this->sample_node();
            this->insert_node_data(new_ip, node_id, next_node);
            next_node->join(node_map[starting_neighbour]);
        }

        this->handle_msgs();
    }
}
