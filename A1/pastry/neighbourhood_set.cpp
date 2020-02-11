#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <neighbourhood_set.h>
#include <iostream>
#include <utils.h>

neighbourhood_set::neighbourhood_set(string node_id)
{
    self_node_id = node_id;
}

void neighbourhood_set::insert_node(string node_id)
{
    if (node_id == self_node_id)
    {
        DEBUG("ERROR CASE EQUAL IN neighbourhood_set::insert_node");
    }
    else
    {
        int pos = lower_bound(neighbours.begin(), neighbours.end(), node_id) - neighbours.begin();

        if (neighbours[pos] == node_id)
        {
            DEBUG("ERROR CASE ALREADY IN neighbourhood_set::insert_node");
        }
        else
        {
            neighbours.insert(neighbours.begin() + pos, node_id);
        }
    }
}

void neighbourhood_set::search_complete(string key, string &closest_node, string &min_dist,int &max_prefix_len)
{
    vector<string>::iterator itr;

    for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
    {
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