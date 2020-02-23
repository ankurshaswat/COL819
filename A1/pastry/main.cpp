#ifndef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                     \
    do                               \
    {                                \
        std::cerr << x << std::endl; \
    } while (0)
#endif

#include <utils.h>
#include <iostream>
#include <coordinator.h>

using namespace std;

int main(int argc, char *argv[])
{

    int num_node_add_queries = atoi(argv[1]);
    int num_node_delete_queries = num_node_add_queries / 2;
    int num_data_add_queries = atoi(argv[2]);
    int num_search_queries = atoi(argv[3]);
    bool enable_logs = bool(atoi(argv[4]));
    int log_node = atoi(argv[5]);
    string log_name = argv[6];

    coordinator *coord = new coordinator(num_search_queries, num_node_add_queries, num_node_delete_queries, num_data_add_queries, enable_logs, log_node, log_name);

    coord->start_simulation();

    return 0;
}