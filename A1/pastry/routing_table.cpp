#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <routing_table.h>
#include <iostream>
#include <utils.h>
#include <cassert>
#include <leaf_set.h>

routing_table::routing_table(string node_id)
{
    this->node_id = node_id;
}

string routing_table::get_routing_match(int len_common_prefix, string key)
{
    int len = this->table.size();
    if (len_common_prefix + 1 > len)
    {
        return "";
    }
    else
    {
        char next_char = key[len_common_prefix];
        int next_char_int = hex_to_int(next_char);
        return table[len_common_prefix][next_char_int];
    }
}

void routing_table::insert_node(string node_id)
{
    assert(node_id != this->node_id);

    size_t len_common_prefix = get_common_prefix_len(node_id, this->node_id);
    char next_char = node_id[len_common_prefix];
    int next_char_int = hex_to_int(next_char);

    while (table.size() < len_common_prefix + 1)
    {
        vector<string> new_vec;
        new_vec.assign(16, "");
        table.push_back(new_vec);
    }

    if (table[len_common_prefix][next_char_int] == "")
    {
        table[len_common_prefix][next_char_int] = node_id;
    }

    // TODO : Check what to do when 2 entries for routing table.
}

void routing_table::initialize_routing_table(routing_table *rtable)
{
    string other_key = rtable->node_id;

    insert_node(other_key);

    vector<vector<string>>::iterator itr_out;
    vector<string>::iterator itr;

    for (itr_out = rtable->table.begin(); itr_out != rtable->table.end(); ++itr_out)
    {
        for (itr = (*itr_out).begin(); itr != (*itr_out).end(); ++itr)
        {
            if (*itr == "")
            {
                continue;
            }
            insert_node(*itr);
        }
    }
}

void routing_table::search_complete(string key, string &closest_node, string &min_dist, int &max_prefix_len)
{
    // Max prefix len match and then lowest distance.
    vector<vector<string>>::iterator itr_out;
    vector<string>::iterator itr;

    for (itr_out = table.begin(); itr_out != table.end(); ++itr_out)
    {
        for (itr = (*itr_out).begin(); itr != (*itr_out).end(); ++itr)
        {
            if (*itr == "")
            {
                continue;
            }
            int prefix_len = get_common_prefix_len(key, *itr);
            string dist = get_dist(*itr, key);
            if (prefix_len > max_prefix_len || (prefix_len == max_prefix_len && dist < min_dist))
            {
                closest_node = *itr;
                min_dist = dist;
                max_prefix_len = prefix_len;
            }
        }
    }
}

void routing_table::search_complete2(string key, string &closest_node, string &min_dist, int min_prefix_len)
{
    // Prefix len more than original and lowest distance.
    vector<vector<string>>::iterator itr_out;
    vector<string>::iterator itr;

    for (itr_out = table.begin(); itr_out != table.end(); ++itr_out)
    {
        for (itr = (*itr_out).begin(); itr != (*itr_out).end(); ++itr)
        {
            if (*itr == "")
            {
                continue;
            }
            int prefix_len = get_common_prefix_len(key, *itr);
            string dist = get_dist(*itr, key);
            if (prefix_len >= min_prefix_len && dist < min_dist)
            {
                closest_node = *itr;
                min_dist = dist;
            }
        }
    }
}