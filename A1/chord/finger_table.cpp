#include <finger_table.h>

node *finger_table::get_node(size_t num)
{
    return nodes[num];
}
node *finger_table::get_start(size_t num)
{
    return starts[num];
}
void finger_table::set_node(size_t num, node *node)
{
    nodes[num] = node;
}