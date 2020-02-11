#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H

#include <string>
#include <vector>

using namespace std;

class routing_table
{
public:
    routing_table(string);
    string get_routing_match(int, string);
    string find_closest_node(size_t, string);
    void insert_node(string node_id);
    void initialize_routing_table(routing_table *rtable);
    void search_complete(string key, string &best_match, string &min_dist,int &);

private:
    string node_id;
    vector<vector<string>> table;
};

#endif