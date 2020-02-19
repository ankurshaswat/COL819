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

    // cout<<generate_random_string(10);

    int num_node_add_queries = atoi(argv[1]);
    int num_node_delete_queries = num_node_add_queries / 2;
    int num_data_add_queries = atoi(argv[2]);
    // int num_search_queries = 1000000;
    int num_search_queries = atoi(argv[3]);
    bool enable_logs = bool(atoi(argv[4]));
    int log_node = atoi(argv[5]);
    string log_name = argv[6];

    cout<<num_node_add_queries<<endl;
    cout<<num_data_add_queries<<endl;
    cout<<num_search_queries<<endl;
    cout<<enable_logs<<endl;
    cout<<log_node<<endl;
    cout<<log_name<<endl;

    coordinator *coord = new coordinator(num_search_queries, num_node_add_queries, num_node_delete_queries, num_data_add_queries, enable_logs, log_node, log_name);

    coord->start_simulation();

    return 0;
}