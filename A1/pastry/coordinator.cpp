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
#include <cassert>

coordinator::coordinator(int num_search_queries, int num_node_add_queries, int num_node_delete_queries, int num_data_add_queries, bool enable_logs, int log_node, string log_name) : num_search_queries(num_search_queries), num_node_add_queries(num_node_add_queries), num_node_delete_queries(num_node_delete_queries), num_data_add_queries(num_data_add_queries), enable_logs(enable_logs), log_node(log_node), log_name(log_name)
{
    log_file.open(log_name + "_before.txt");
    // log_file_before.open(log_name + "_before");
    // log_file_after.open(log_name + "_after");

    // num_keys_file_before.open(log_name + "_before");
    // num_keys_file_after.open(log_name + "_after");
    // DEBUG("Num nodes in simulation = " << num_nodes);
}

node *coordinator::get_node(string node_id)
{
    assert(node_id_map[node_id]);
    return node_map[node_id];
}

void coordinator::start_simulation()
{

    for (size_t i = 0; i < this->num_node_add_queries; i++)
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

        node *next_node;

        if ((i + 1) == log_node)
        {
            next_node = new node(new_ip, node_id, this, enable_logs);
        }
        else
        {
            next_node = new node(new_ip, node_id, this, false);
        }

        DEBUG("Node:\"" << i << "\" NodeID:\"" << node_id << "\" IP:\"" << new_ip << "\"");

        string starting_neighbour = this->sample_close_neighbour(new_ip);

        this->insert_node_data(new_ip, node_id, next_node);

        if (starting_neighbour != "")
        {
            msg_type joining_msg = next_node->join_network(starting_neighbour);
            this->msg_q.push(joining_msg);
        }

        this->handle_msgs();

        num_nodes++;
    }

    for (size_t i = 0; i < num_data_add_queries; i++)
    {
        string data_title, value;
        data_title = generate_random_string(10);
        value = data_title + " val";

        int node_number = rand() % node_list.size();
        string node_id = node_list[node_number];
        node *node = get_node(node_id);

        node->put(data_title, value);
        data.push_back(data_title);
        num_data_elements++;
    }

    for (size_t i = 0; i < num_search_queries; i++)
    {
        // cout << i << endl;
        int data_num = rand() % data.size();
        string data_title = data[data_num];

        int node_number = rand() % node_list.size();
        string node_id = node_list[node_number];
        node *node = get_node(node_id);

        string res = node->get(data_title);
        // cout << res << endl;
    }

    log_file.close();

    ofstream key_pdf;
    key_pdf.open(log_name + "_keys_before.txt");
    for (size_t i = 0; i < node_list.size(); i++)
    {
        node *node = get_node(node_list[i]);
        key_pdf << node->get_num_keys() << "\n";
    }
    key_pdf.close();

    log_file.open(log_name + "_after.txt");

    for (size_t i = 0; i < num_node_delete_queries; i++)
    {
        // cout << i << endl;

        int ip_number = rand() % ip_list.size();
        string ip = ip_list[ip_number];
        string node_id = upper_md5(ip);
        node *node = get_node(node_id);

        num_data_elements -= node->get_num_keys();
        // cout << node_id << endl;

        // Delete this node
        node->delete_self();
        remove_node_data(ip, node_id);
        delete node;
    }

    key_pdf.open(log_name + "_keys_after.txt");
    for (size_t i = 0; i < node_list.size(); i++)
    {
        node *node = get_node(node_list[i]);
        key_pdf << node->get_num_keys() << "\n";
    }
    key_pdf.close();

    for (size_t i = 0; i < num_search_queries; i++)
    {
        // cout << i << endl;
        int data_num = rand() % data.size();
        string data_title = data[data_num];

        int node_number = rand() % node_list.size();
        string node_id = node_list[node_number];
        node *node = get_node(node_id);

        string res = node->get(data_title);
        // cout << res << endl;
    }

    if (enable_logs)
    {
        cout << "Total number of nodes : " << num_nodes << "." << endl;
        cout << "Total number of data elements : " << num_data_elements << "." << endl;
        cout << "Total search queries : " << num_search_queries << "." << endl;
        cout << "Total node add queries : " << num_node_add_queries << "." << endl;
        cout << "Total node delete queries : " << num_node_delete_queries << "." << endl;
        cout << "Total data add queries : " << num_data_add_queries << "." << endl
             << endl;
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
    node_list.push_back(node_id);
    this->node_id_map[node_id] = true;
    this->node_map[node_id] = new_node;
}

void coordinator::remove_node_data(string ip, string node_id)
{
    node_ip_map.erase(ip);
    node_id_map.erase(node_id);
    node_map.erase(node_id);

    ip_list.erase(remove(ip_list.begin(), ip_list.end(), ip), ip_list.end());
    node_list.erase(remove(node_list.begin(), node_list.end(), node_id), node_list.end());

    num_nodes--;
}

bool coordinator::check_exist(string node_id)
{
    return node_id_map[node_id];
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

coordinator::~coordinator()
{
    log_file.close();
}

void coordinator::write_hop_num(int num_hops)
{
    log_file << num_hops << "\n";
}