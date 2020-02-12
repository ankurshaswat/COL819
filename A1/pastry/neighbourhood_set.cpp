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
#include <cassert>

neighbourhood_set::neighbourhood_set(string node_id)
{
    self_node_id = node_id;
}

void neighbourhood_set::insert_node(string node_id)
{
    assert(node_id != self_node_id);
    neighbours.insert(node_id);
}

void neighbourhood_set::search_complete(string key, string &closest_node, string &min_dist, int &max_prefix_len)
{
    // Max prefix len match and then lowest distance.
    set<string>::iterator itr;

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

void neighbourhood_set::search_complete2(string key, string &closest_node, string &min_dist, int min_prefix_len)
{
    // Prefix len more than original and lowest distance.
    set<string>::iterator itr;

    for (itr = neighbours.begin(); itr != neighbours.end(); ++itr)
    {
        int prefix_len = get_common_prefix_len(key, *itr);
        string dist = get_dist(*itr, key);
        if (prefix_len >= min_prefix_len && dist < min_dist)
        {
            closest_node = *itr;
            min_dist = dist;
        }
    }
}