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

int main()
{

    // cout<<generate_random_string(10);

    int num_node_add_queries = 100;
    int num_node_delete_queries = num_node_add_queries / 2;
    int num_data_add_queries = 10000;
    // int num_search_queries = 1000000;
    int num_search_queries = 10000;
    bool enable_logs = true;
    int log_node = 10;

    coordinator *coord = new coordinator(num_search_queries, num_node_add_queries, num_node_delete_queries, num_data_add_queries, enable_logs, log_node);

    coord->start_simulation();

    return 0;
}