#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <finger_table.h>
#include <math.h>
#include <iostream>

using namespace std;

// #define NUM_LEAVES 4
#define NUM_LEAVES 32
// #define limit 16U
#define limit 4294967296U

finger_table::finger_table(size_t node_id) : node_id(node_id)
{
    starts.assign(NUM_LEAVES, 0);
    nodes.assign(NUM_LEAVES, 0);

    for (int i = 1; i <= NUM_LEAVES; i++)
    {
        size_t pow_res = pow(2, i - 1);
        starts[i - 1] = (node_id + pow_res) % limit;
    }
}

size_t finger_table::get_node(size_t num)
{
    return nodes[num - 1];
}
size_t finger_table::get_start(size_t num)
{
    return starts[num - 1];
}
void finger_table::set_node(size_t num, size_t node)
{
    DEBUG("Updating finger table " << node_id << " row " << num << " from " << nodes[num - 1] << " to " << node);
    nodes[num - 1] = node;
}