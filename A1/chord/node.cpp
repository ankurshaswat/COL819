#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <node.h>
#include <math.h>
#include <coordinator.h>
#include <iostream>

using namespace std;

#define NUM_LEAVES 4

node::node(string node_ip, size_t node_id, coordinator *coord, bool enable_logs) : node_id(node_id), node_ip(node_ip), coord(coord), enable_logs(enable_logs)
{
    this->ftable = new finger_table(node_id);
}

node *node::get_successor()
{
    return coord->get_node(ftable->get_node(1));
}

node *node::find_successor(size_t id)
{
    DEBUG("Finding pred for " << id);
    node *n_dash = find_predecessor(id);
    DEBUG("Found pred " << n_dash->node_id);
    return n_dash->get_successor();
}

node *node::find_predecessor(size_t id)
{
    // DEBUG("Finding predecessor " << id);

    // if (id == this->node_id)
    // {
    //     return coord->get_node(predecessor);
    // }

    node *n_dash = this;

    if (this->node_id == this->get_successor()->node_id)
    {
        return coord->get_node(predecessor);
    }

    // while (!(id > n_dash->node_id && id <= n_dash->get_successor()->node_id))
    while (!(id == n_dash->get_successor()->node_id || in_range(id, n_dash->node_id, n_dash->get_successor()->node_id)))
    {
        if (id == n_dash->node_id)
        {
            return coord->get_node(n_dash->predecessor);
        }
        // if (n_dash->node_id == n_dash->get_successor()->node_id)
        // {
        //     return n_dash;
        // }
        node *new_finger = n_dash->closest_preceding_finger(id);
        if (new_finger == n_dash)
        {
            break;
        }
        n_dash = new_finger;
    }

    return n_dash;
}

node *node::closest_preceding_finger(size_t id)
{
    DEBUG("Finding closest preceding finger " << id);
    // if (id == this->node_id)
    // {
    //     return coord->get_node(predecessor);
    // }
    for (size_t i = NUM_LEAVES; i >= 1; i--)
    {
        // if (ftable->get_node(i) > this && ftable->get_node(i)->node_id < id)
        if (in_range(ftable->get_node(i), this->node_id, id))
        {
            return coord->get_node(ftable->get_node(i));
        }
    }
    return this;
}

void node::join(node *existing_node)
{
    if (existing_node != NULL)
    {
        init_finger_table(existing_node);
        update_others();
        // move keys in (predecessor,n] from successor
    }
    else
    {
        //* First node
        for (size_t i = 1; i <= NUM_LEAVES; i++)
        {
            //* finger[i].node = n;
            ftable->set_node(i, this->node_id);
        }
        predecessor = this->node_id;
    }
}

void node::init_finger_table(node *existing_node)
{
    //* finger[1].node = new_node->find_successor(finger[1].start);
    size_t start_1_id = ftable->get_start(1);
    node *successor = existing_node->find_successor(start_1_id);
    ftable->set_node(1, successor->node_id);

    predecessor = get_successor()->predecessor;
    get_successor()->predecessor = this->node_id;

    // Line not in paper
    coord->get_node(predecessor)->ftable->set_node(1, this->node_id);

    for (size_t i = 1; i <= NUM_LEAVES - 1; i++)
    {
        // if (ftable->get_start(i + 1) >= this && ftable->get_start(i + 1) < ftable->get_node(i))
        if (ftable->get_start(i + 1) == this->node_id || in_range(ftable->get_start(i + 1), this->node_id, ftable->get_node(i)))
        {
            //* finger[i + 1].node = finger[i].node;
            ftable->set_node(i + 1, ftable->get_node(i));
        }
        else
        {
            // finger[i + 1].node = new_node->find_successor(finger[i + 1].start);
            size_t next_start = ftable->get_start(i + 1);
            node *next_succ = existing_node->find_successor(next_start);
            ftable->set_node(i + 1, next_succ->node_id);
        }
    }
}

void node::update_others()
{
    for (size_t i = 1; i <= NUM_LEAVES; i++)
    {
        size_t sub_res = circular_subtract(node_id, pow(2, i - 1));
        DEBUG("Finding pred to update for " << sub_res);
        node *p = find_predecessor(sub_res);
        DEBUG("Found pred " << p->node_id);
        if (p == this)
        {
            continue;
        }
        p->update_finger_table(this, i);
    }
}

void node::update_finger_table(node *s, size_t i)
{
    if (s == this)
    {
        return;
    }
    // If s is i'th finger of n
    if (this->node_id != ftable->get_node(i))
    {
        // if (s >= this && s < ftable->get_node(i))
        // DEBUG("Update finger table " << s->node_id << " E "
        // << "[" << this->node_id << "," << ftable->get_node(i) << ")");
        if (s->node_id == this->node_id || in_range(s->node_id, this->node_id, ftable->get_node(i)))
        {
            // finger[i].node = s;
            ftable->set_node(i, s->node_id);
            node *p = coord->get_node(predecessor);
            p->update_finger_table(s, i);
        }
    }
    else
    {
        if (s->node_id == ftable->get_start(i) || in_range(s->node_id, ftable->get_start(i), this->node_id))
        {
            ftable->set_node(i, s->node_id);
            node *p = coord->get_node(predecessor);
            p->update_finger_table(s, i);
        }
    }
}

string node::retreive_data(size_t key)
{
    if (data_store.find(key) == data_store.end())
    {
        return "";
    }
    else
    {
        return data_store[key];
    }
}

void node::store_data(size_t key, string val)
{
    data_store[key] = val;
}

int node::get_num_keys()
{
    return data_store.size();
}

void node::notify_removal(node *removed_node)
{
    // if (removed_node == get_successor())
    // {
    //     ftable->set_node(1, removed_node->get_successor());
    // }
    // else if (removed_node == this->predecessor)
    // {
    //     this->predecessor = removed_node->predecessor;
    // }
    // else
    // {
    DEBUG("node::notify_removal UNFINISHED CASE");
    // }
}

void node::delete_self()
{
    DEBUG("node::delete_self UNFINISHED CASE");

    // node *succ = get_successor();

    // unordered_map<size_t, string>::iterator itr;

    // // Transfer all keys to next node
    // for (itr = data_store.begin(); itr != data_store.end(); ++itr)
    // {
    //     succ->store_data((*itr).first, (*itr).second);
    // }

    // // Notify predecessor and successor of removal
    // get_successor()->notify_removal(get_successor());
    // predecessor->notify_removal(predecessor);
}

string node::get(string key)
{
    size_t key_identifier = hash_custom(key);
    DEBUG("Get request received at " << node_id << " for " << key_identifier);
    return get(key_identifier);
}

string node::get(size_t key)
{
    node *successor = find_successor(key);
    if (successor == this)
    {
        DEBUG("Retreiving  at " << node_id << " " << key);
        return retreive_data(key);
    }
    else
    {
        DEBUG("Forwarding get request to " << successor->node_id << " for " << key);
        return successor->get(key);
    }
}

void node::put(string key, string val)
{
    size_t key_identifier = hash_custom(key);
    DEBUG("Put request received at " << node_id << " for " << key_identifier << " " << val);
    put(key_identifier, val);
}

void node::put(size_t key, string val)
{
    node *successor = find_successor(key);
    if (successor == this)
    {
        DEBUG("Storing  at " << node_id << " " << key << " " << val);
        return store_data(key, val);
    }
    else
    {
        DEBUG("Forwarding put request to " << successor->node_id << " for " << key << " " << val);
        return successor->put(key, val);
    }
}