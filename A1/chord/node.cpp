#include <node.h>
#include <math.h>

#define NUM_LEAVES 4

node::node(string node_id, string node_ip)
{
}

node *node::find_successor(node *)
{
}

node *node::find_predecessor(size_t)
{
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

void node::init_finger_table(node *new_node)
{
    //* finger[1].node = new_node->find_successor(finger[1].start);
    ftable->set_node(1, new_node->find_successor(ftable->get_start(1)));

    predecessor = successor->predecessor;
    successor->predecessor = this;
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
            ftable->set_node(i + 1, new_node->find_successor(ftable->get_start(i + 1)));
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