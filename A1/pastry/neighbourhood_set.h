#ifndef NEIGHBOURHOOD_SET_H
#define NEIGHBOURHOOD_SET_H

#include <string>
#include <vector>

using namespace std;

class neighbourhood_set
{
public:
    neighbourhood_set(string);
    string find_closest_node(int len_common_prefix, string key);
    void insert_node(string node_id);
    void search_complete(string key, string &best_match, string &min_dist,int &);

private:
    vector<string> neighbours;
    string self_node_id;
};

#endif