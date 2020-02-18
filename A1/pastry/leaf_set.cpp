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
    assert(!incomplete());

    string left_extreme = "";
    string right_extreme = "";

    left_extreme = *left_set.begin();
    right_extreme = *(prev(right_set.end()));

    if (target >= left_extreme && target <= right_extreme)
    {
        return true;
    }

    return false;
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
}

string leaf_set::find_closest_leaf2(string key)
{
    assert(key != self_node_id);

    string min_dist = get_dist(key, self_node_id);
    string closest_node = self_node_id;

    set<string>::iterator itr;

    if (key < self_node_id)
    {
        for (itr = left_set.begin(); itr != left_set.end(); ++itr)
        {
            string dist = get_dist(*itr, key);
            if (dist < min_dist)
            {
                closest_node = *itr;
                min_dist = dist;
            }
        }
    }
    else
    {
        for (itr = right_set.begin(); itr != right_set.end(); ++itr)
        {
            string dist = get_dist(*itr, key);
            if (dist < min_dist)
            {
                closest_node = *itr;
                min_dist = dist;
            }
        }
    }

    return closest_node;
}

bool leaf_set::incomplete()
{
    if (left_set.size() < 16 || right_set.size() < 16)
    {
        return true;
    }

    return false;
}

string leaf_set::search_incomplete_set(string key)
{

    string closest_node = self_node_id;
    string min_dist = get_dist(key, self_node_id);

    set<string>::iterator itr;

    for (itr = left_set.begin(); itr != left_set.end(); ++itr)
    {
        string dist = get_dist(*itr, key);
        if (dist < min_dist)
        {
            closest_node = *itr;
            min_dist = dist;
        }
    }

    for (itr = right_set.begin(); itr != right_set.end(); ++itr)
    {
        string dist = get_dist(*itr, key);
        if (dist < min_dist)
        {
            closest_node = *itr;
            min_dist = dist;
        }
    }

    return closest_node;
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

void leaf_set::search_complete2(string key, string &closest_node, string &min_dist, int min_prefix_len)
{
    set<string>::iterator itr;

    for (itr = left_set.begin(); itr != left_set.end(); ++itr)
    {
        int prefix_len = get_common_prefix_len(key, *itr);
        string dist = get_dist(*itr, key);
        if (prefix_len >= min_prefix_len && dist < min_dist)
        {
            closest_node = *itr;
            min_dist = dist;
        }
    }

    for (itr = right_set.begin(); itr != right_set.end(); ++itr)
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

void leaf_set::insert_node(string node_id)
{
    assert(node_id != self_node_id);

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
            right_set.erase(*prev(right_set.end()));
        }
    }
}

void leaf_set::initialize_leaf_set(leaf_set *old_ls)
{
    set<string>::iterator itr;
    // size_t initial_size = left_set.size() + right_set.size();

    for (itr = old_ls->left_set.begin(); itr != old_ls->left_set.end(); ++itr)
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
    for (itr = old_ls->right_set.begin(); itr != old_ls->right_set.end(); ++itr)
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

    // size_t final_size = left_set.size() + right_set.size();

    // cout << final_size - initial_size << " increase in size of leafset." << endl;
}

vector<string> leaf_set::get_leaves()
{
    vector<string> leaves;

    set<string>::iterator itr;

    for (itr = left_set.begin(); itr != left_set.end(); ++itr)
    {
        leaves.push_back(*itr);
    }

    for (itr = right_set.begin(); itr != right_set.end(); ++itr)
    {
        leaves.push_back(*itr);
    }
    return leaves;
}

// void leaf_set::add_inform_msgs(vector<msg_type> &replies)
// {
//     set<string>::iterator itr;

//     for (itr = left_set.begin(); itr != left_set.end(); ++itr)
//     {
//         msg_type inform_msg;

//         inform_msg.type = "inform";
//         inform_msg.sender_id = self_node_id;
//         inform_msg.target_id = *itr;
//         inform_msg.key = *itr;
//         inform_msg.value = "";

//         replies.push_back(inform_msg);
//     }

//     for (itr = right_set.begin(); itr != right_set.end(); ++itr)
//     {
//         msg_type inform_msg;

//         inform_msg.type = "inform";
//         inform_msg.sender_id = self_node_id;
//         inform_msg.target_id = *itr;
//         inform_msg.key = *itr;
//         inform_msg.value = "";

//         replies.push_back(inform_msg);
//     }
// }

int leaf_set::size()
{
    return left_set.size() + right_set.size();
}

string leaf_set::remove(string node_id)
{
    // Returns the node id of leaf set needed to complete the leaf set after removal.
    assert(node_id != self_node_id);

    if (node_id < self_node_id)
    {
        if (left_set.find(node_id) == left_set.end())
            return "";
        else
        {
            left_set.erase(node_id);

            if (left_set.size() == 15)
                return *left_set.begin();
            else
                return "";
        }
    }
    else
    {
        if (right_set.find(node_id) == right_set.end())
            return "";
        else
        {
            right_set.erase(node_id);

            if (right_set.size() == 15)
                return *(prev(right_set.end()));
            else
                return "";
        }
    }
}