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

#define NUM_LEAVES 4

node::node(string node_ip, size_t node_id)
{
    this->node_id = node_id;
    this->node_ip = node_ip;
}

node *node::get_successor()
{
    return ftable->get_node(1);
}

node *node::find_successor(size_t id)
{
    node *n_dash = find_predecessor(id);
    return n_dash->get_successor();
}

node *node::find_predecessor(size_t id)
{
    node *n_dash = this;
    while (!(id > n_dash->node_id && id <= n_dash->get_successor()->node_id))
    {
        n_dash = n_dash->closest_preceding_finger(id);
    }

    return n_dash;
}

node *node::closest_preceding_finger(size_t id)
{
    for (size_t i = NUM_LEAVES; i >= 1; i--)
    {
        if (ftable->get_node(i) > this && ftable->get_node(i)->node_id < id)
        {
            return ftable->get_node(i);
        }
    }
    return this;
}

void node::join(node *new_node)
{
    if (new_node != NULL)
    {
        init_finger_table(new_node);
        update_others();
        // move keys in (predecessor,n] from successor
    }
    else
    {
        //* First node
        for (size_t i = 1; i <= NUM_LEAVES; i++)
        {
            //* finger[i].node = n;
            ftable->set_node(i, this);
        }
    }
}

void node::notify_removal(node *removed_node)
{
    if (removed_node == get_successor())
    {
        ftable->set_node(1, removed_node->get_successor());
    }
    else if (removed_node == this->predecessor)
    {
        this->predecessor = removed_node->predecessor;
    } else {
        DEBUG("node::notify_removal UNFINISHED CASE");
    }
}

void node::delete_self()
{
    node *succ = get_successor();

    unordered_map<size_t, string>::iterator itr;

    // Transfer all keys to next node
    for (itr = data_store.begin(); itr != data_store.end(); ++itr)
    {
        succ->store_data((*itr).first, (*itr).second);
    }

    // Notify predecessor and successor of removal
    get_successor()->notify_removal(get_successor());
    predecessor->notify_removal(predecessor);
}

string node::get(string key)
{
    size_t key_identifier = hash_custom(key);
    return get(key_identifier);
}

string node::get(size_t key)
{
    node *successor = find_successor(key);
    if (successor == this)
    {
        return retreive_data(key);
    }
    else
    {
        return successor->get(key);
    }
}

void node::put(string key, string val)
{
    size_t key_identifier = hash_custom(key);
    put(key_identifier, val);
}

void node::put(size_t key, string val)
{
    node *successor = find_successor(key);
    if (successor == this)
    {
        return store_data(key, val);
    }
    else
    {
        return successor->put(key, val);
    }
}

void node::init_finger_table(node *new_node)
{
    //* finger[1].node = new_node->find_successor(finger[1].start);
    ftable->set_node(1, new_node->find_successor(ftable->get_start(1)->node_id));

    predecessor = get_successor()->predecessor;
    get_successor()->predecessor = this;
    for (size_t i = 1; i <= NUM_LEAVES - 1; i++)
    {
        if (ftable->get_start(i + 1) >= this && ftable->get_start(i + 1) < ftable->get_node(i))
        {
            //* finger[i + 1].node = finger[i].node;
            ftable->set_node(i + 1, ftable->get_node(i));
        }
        else
        {
            // finger[i + 1].node = new_node->find_successor(finger[i + 1].start);
            ftable->set_node(i + 1, new_node->find_successor(ftable->get_start(i + 1)->node_id));
        }
    }
}

void node::update_others()
{
    for (size_t i = 1; i <= NUM_LEAVES; i++)
    {
        node *p = find_predecessor(node_id - pow(2, i - 1));
        p->update_finger_table(this, i);
    }
}

void node::update_finger_table(node *s, size_t i)
{
    if (s >= this && s < ftable->get_node(i))
    {
        // finger[i].node = s;
        ftable->set_node(i, s);
        node *p = predecessor;
        p->update_finger_table(s, i);
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

bool node::operator<(const node &d)
{
    if (node_id < d.node_id)
    {
        return true;
    }
    return false;
}
bool node::operator==(const node &d)
{
    if (node_id == d.node_id)
    {
        return true;
    }
    return false;
}

bool node::operator<=(const node &d)
{
    if (node_id <= d.node_id)
    {
        return true;
    }
    return false;
}

bool node::operator>(const node &d)
{
    if (node_id > d.node_id)
    {
        return true;
    }
    return false;
}

bool node::operator>=(const node &d)
{
    if (node_id >= d.node_id)
    {
        return true;
    }
    return false;
}