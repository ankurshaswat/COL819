#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <leaf_set.h>
#include <utils.h>
#include <iostream>
#include <cassert>

leaf_set::leaf_set(string node_id)
{
    this->self_node_id = node_id;
}

bool leaf_set::check_in_leaf_set(string target)
{
    string left_extreme = "";
    string right_extreme = "";

    // if (left_set.empty() || right_set.empty())
    // {
    //     return false;
    // }

    // if (left_set.empty())
    // {
    //     left_extreme = self_node_id;
    // }
    // else
    // {
    //     left_extreme = *left_set.begin();
    // }

    // if (right_set.empty())
    // {
    //     right_extreme = self_node_id;
    // }
    // else
    // {
    //     right_extreme = *(--right_set.end());
    // }

    // if (left_extreme == right_extreme)
    // {
    //     return false;
    // }

    left_extreme = *left_set.begin();
    right_extreme = *(--right_set.end());

    if (target >= left_extreme && target <= right_extreme)
    {
        return true;
    }

    return false;

    // if ((left_leaves.size() != 0 && target < left_leaves[0]) || (left_leaves.size() == 0 && target < self_node_id))
    // {
    //     return false;
    // }

    // int len = right_leaves.size();

    // if ((right_leaves.size() != 0 && target > right_leaves[len - 1]) || (right_leaves.size() == 0 && target > self_node_id))
    // {
    //     return false;
    // }

    // return true;
}

string leaf_set::find_closest_leaf(string key)
{
    assert(key != self_node_id);

    string min_dist = get_dist(key, self_node_id);
    int max_prefix_len = get_common_prefix_len(key, self_node_id);
    string closest_node = self_node_id;

    set<string>::iterator itr;

    if (key < self_node_id)
    {
        for (itr = left_set.begin(); itr != left_set.end(); ++itr)
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
    else
    {
        for (itr = right_set.begin(); itr != right_set.end(); ++itr)
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

    return closest_node;
    // Binary Search over leaves.
    // DEBUG("leaf_set::find_closest_leaf INCOMPLETE FUNCTION");
    // return "";
}

bool leaf_set::incomplete()
{
    if (left_set.size() < 16 || right_set.size() < 16)
    {
        return true;
    }

    return false;
}

string leaf_set::search_complete(string key)
{
    string best_match_node_id = self_node_id;
    string lowest_hex_distance = get_dist(key, self_node_id);
    int max_prefix_len = get_common_prefix_len(key, self_node_id);
    search_complete(key, best_match_node_id, lowest_hex_distance, max_prefix_len);

    return best_match_node_id;
}

void leaf_set::search_complete(string key, string &closest_node, string &min_dist, int &max_prefix_len)
{
    set<string>::iterator itr;

    for (itr = left_set.begin(); itr != left_set.end(); ++itr)
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

    for (itr = right_set.begin(); itr != right_set.end(); ++itr)
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

void leaf_set::insert_node(string node_id)
{
    if (node_id == self_node_id)
    {
        DEBUG("ERROR CASE EQUAL IN leaf_set::insert_node");
    }

    if (node_id < self_node_id)
    {
        left_set.insert(node_id);
        if (left_set.size() > 16)
        {
            left_set.erase(*left_set.begin());
        }
    }
    else
    {
        right_set.insert(node_id);
        if (right_set.size() > 16)
        {
            right_set.erase(*--right_set.end());
        }
    }
}

void leaf_set::initialize_leaf_set(leaf_set *old_ls)
{
    set<string>::iterator itr;

    for (itr = left_set.begin(); itr != left_set.end(); ++itr)
    {
        string new_node_id = *itr;
        if (new_node_id == self_node_id)
        {
            continue;
        }
        else
        {
            insert_node(new_node_id);
        }
    }
    insert_node(old_ls->self_node_id);
    for (itr = right_set.begin(); itr != right_set.end(); ++itr)
    {
        string new_node_id = *itr;
        if (new_node_id == self_node_id)
        {
            continue;
        }
        else
        {
            insert_node(new_node_id);
        }
    }
}

void leaf_set::add_inform_msgs(vector<msg_type> &replies)
{
    set<string>::iterator itr;

    for (itr = left_set.begin(); itr != left_set.end(); ++itr)
    {
        msg_type inform_msg;

        inform_msg.type = "inform";
        inform_msg.sender_id = self_node_id;
        inform_msg.target_id = *itr;
        inform_msg.key = *itr;
        inform_msg.value = "";

        replies.push_back(inform_msg);
    }

    for (itr = right_set.begin(); itr != right_set.end(); ++itr)
    {
        msg_type inform_msg;

        inform_msg.type = "inform";
        inform_msg.sender_id = self_node_id;
        inform_msg.target_id = *itr;
        inform_msg.key = *itr;
        inform_msg.value = "";

        replies.push_back(inform_msg);
    }
}