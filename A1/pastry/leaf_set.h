#ifndef LEAF_SET_H
#define LEAF_SET_H

#include <string>
#include <vector>
#include <utils.h>
#include <set>

using namespace std;

class leaf_set
{
public:
    leaf_set(string);
    bool check_in_leaf_set(string);
    string find_closest_leaf(string);
    string find_closest_leaf2(string);
    string find_closest_leaf(size_t, string);
    void insert_node(string node_id);
    void initialize_leaf_set(leaf_set *);
    void add_inform_msgs(vector<msg_type> &);
    bool incomplete();
    string search_complete(string);
    string search_incomplete_set(string);
    void search_complete(string, string &, string &, int &);
    void search_complete2(string, string &, string &, int);
    vector<string> get_leaves();
    int size();
    string remove(string node_id);

private:
    string self_node_id;
    // vector<string> left_leaves, right_leaves;
    set<string> left_set, right_set;
};

#endif