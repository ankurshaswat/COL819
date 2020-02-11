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

// string routing_table::find_closest_node(size_t len_common_prefix, string key)
// {
//     size_t len = this->table.size();
//     if (len_common_prefix + 1 > len)
//     {
//         return "";
//     }
//     else
//     {
//         string best_match_node_id = "";
//         string lowest_hex_distance = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
//         vector<string>::iterator itr;

//         for (itr = table[len_common_prefix].begin(); itr != table[len_common_prefix].end(); ++itr)
//         {
//             string prefix = get_common_prefix(*itr, key);
//             if (prefix.length() >= len_common_prefix)
//             {
//                 string distance = get_dist(key, *itr);
//                 if (distance < lowest_hex_distance)
//                 {
//                     best_match_node_id = *itr;
//                     lowest_hex_distance = distance;
//                 }
//             }
//         }

//         return best_match_node_id;
//         // DEBUG("routing_table::get_routing_match INCOMPLETE FUNCTION");
//         // return "";
//     }
// }

void routing_table::insert_node(string node_id)
{
    if (node_id == this->node_id)
    {
        DEBUG("routing_table::insert_node EQUAL CASE ERROR");
    }
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
    // else
    // {
    // DEBUG("routing_table::insert_node CASE DONT KNOW WHAT TO DO");
    // }
}

void routing_table::initialize_routing_table(routing_table *rtable)
{
    string other_key = rtable->node_id;
    size_t len_common_prefix = get_common_prefix_len(node_id, other_key);

    for (size_t i = 0; i < rtable->table.size() && i <= len_common_prefix; i++)
    {

        if (table.size() < i + 1)
        {
            vector<string> new_vec;
            new_vec.assign(16, "");
            table.push_back(new_vec);
        }

        for (int j = 0; j < 16; j++)
        {
            if (rtable->table[i][j] != "")
            {
                table[i][j] = rtable->table[i][j];
            }
        }
        table[i][hex_to_int(node_id[i])] = "";
    }

    // TODO : Search what to do when both places have entry.
}

void routing_table::search_complete(string key, string &closest_node, string &min_dist, int &max_prefix_len)
{
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