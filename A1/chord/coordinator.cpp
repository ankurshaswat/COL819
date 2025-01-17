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
#include <cassert>
#include <algorithm>

using namespace std;

coordinator::coordinator(int num_search_queries, int num_node_add_queries, int num_node_delete_queries, int num_data_add_queries, bool enable_logs, int log_node, string log_name) : num_search_queries(num_search_queries), num_node_add_queries(num_node_add_queries), num_node_delete_queries(num_node_delete_queries), num_data_add_queries(num_data_add_queries), log_node(log_node), enable_logs(enable_logs), log_name(log_name)
{
    log_file.open(log_name + "_before.txt");
}

node *coordinator::get_node(size_t node_id)
{
    assert(node_id_map[node_id]);
    return node_map[node_id];
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

bool coordinator::check_exist(size_t node_id)
{
    return !check_free_node_id(node_id);
}

void coordinator::insert_node_data(string ip, size_t node_id, node *new_node)
{
    this->node_ip_map[ip] = true;
    this->node_list.push_back(node_id);
    this->ip_list.push_back(ip);
    this->node_id_map[node_id] = true;
    this->node_map[node_id] = new_node;
}

void coordinator::remove_node_data(string ip, size_t node_id)
{
    node_ip_map.erase(ip);
    node_id_map.erase(node_id);
    node_map.erase(node_id);

    ip_list.erase(remove(ip_list.begin(), ip_list.end(), ip), ip_list.end());
    node_list.erase(remove(node_list.begin(), node_list.end(), node_id), node_list.end());

    num_nodes--;
}

size_t coordinator::sample_node()
{
    size_t random_node = rand() % node_list.size();
    return node_list[random_node];
}

// void coordinator::start_simulation()
// {

//     for (int i = 0; i < this->num_nodes; i++)
//     {
//         string new_ip;
//         size_t node_id;

//         while (true)
//         {
//             new_ip = get_random_ip();
//             node_id = hash_custom(new_ip);
//             if (!this->check_free_ip(new_ip) || !this->check_free_node_id(node_id))
//             {
//                 DEBUG("Continuing");
//                 continue;
//             }
//             else
//             {
//                 break;
//             }
//         }
//         node *next_node = new node(new_ip, node_id);

//         cout << "Node:\"" << i << "\" NodeID:\"" << node_id << "\" IP:\"" << new_ip << "\"" << endl;

//         if (node_list.size() == 0)
//         {
//             next_node->join(NULL);
//         }
//         else
//         {
//             size_t starting_neighbour = this->sample_node();
//             this->insert_node_data(new_ip, node_id, next_node);
//             next_node->join(node_map[starting_neighbour]);
//         }

//         this->handle_msgs();
//     }
// }

void coordinator::start_simulation()
{

    for (size_t i = 0; i < this->num_node_add_queries; i++)
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

        node *next_node;

        next_node = new node(new_ip, node_id, this, enable_logs);

        DEBUG("Node:\"" << i << "\" NodeID:\"" << node_id << "\" IP:\"" << new_ip << "\"");

        this->insert_node_data(new_ip, node_id, next_node);

        if (node_list.size() == 1)
        {
            next_node->join(NULL);
        }
        else
        {
            size_t starting_neighbour = this->sample_node();
            while (starting_neighbour == node_id)
            {
                starting_neighbour = this->sample_node();
            }
            next_node->join(node_map[starting_neighbour]);
        }

        num_nodes++;
    }

    if (enable_logs)
    {

        // * Print routing tables
        // for (size_t i = 0; i < node_list.size(); i++)
        // {
        //     string node_2_log_id = node_list[i];
        //     node *node_2_log = node_map[node_2_log_id];
        //     node_2_log->print_routing_table();
        // }

        size_t node_2_log_id = node_list[log_node];
        node *node_2_log = node_map[node_2_log_id];
        node_2_log->print_finger_table();
    }

    for (size_t i = 0; i < num_data_add_queries; i++)
    {
        string data_title, value;
        data_title = generate_random_string(10);
        value = data_title + " val";

        int node_number = rand() % node_list.size();
        size_t node_id = node_list[node_number];
        node *node = get_node(node_id);

        node->put(data_title, value);
        data.push_back(data_title);
        num_data_elements++;

        // DEBUG("Data put :\"" << i << "\"");
    }

    for (size_t i = 0; i < num_search_queries; i++)
    {
        // cout << i << endl;
        int data_num = rand() % data.size();
        string data_title = data[data_num];

        int node_number = rand() % node_list.size();
        size_t node_id = node_list[node_number];
        node *node = get_node(node_id);

        string res = node->get(data_title);
        // cout << res << endl;
        DEBUG("Data search :\"" << i << "\" Res:\"" << res << "\"");
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
        size_t node_id = hash_custom(ip);
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
        size_t node_id = node_list[node_number];
        node *node = get_node(node_id);

        string res = node->get(data_title);
        // cout << res << endl;
        DEBUG("Data search :\"" << i << "\" Res:\"" << res << "\"");
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

void coordinator::write_hop_num(int num_hops)
{
    log_file << num_hops << "\n";
}