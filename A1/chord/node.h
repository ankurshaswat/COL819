#ifndef NODE_H
#define NODE_H

#include <string>
#include <finger_table.h>
#include <utils.h>
#include <vector>
#include <unordered_map>

using namespace std;

class finger_table;
class node
{
public:
    node(string, size_t);
    void join(node *);
    void init_finger_table(node *);
    void update_others();
    void update_finger_table(node *, size_t);
    node *find_successor(size_t);
    node *find_predecessor(size_t);
    // string get_finger_start(int);
    bool operator<(const node &);
    bool operator==(const node &);
    bool operator<=(const node &);
    bool operator>(const node &);
    bool operator>=(const node &);
    node *closest_preceding_finger(size_t id);
    // vector<msg_type> receive_send_msg(msg_type);
    string retreive_data(size_t key);
    void store_data(size_t key, string val);
    void delete_self();
    string get(string key);
    string get(size_t key);
    void put(string key, string val);
    void put(size_t key, string val);
    node *get_successor();
    void notify_removal(node *);

private:
    finger_table *ftable;
    node *predecessor;
    size_t node_id;
    string node_ip;
    unordered_map<size_t, string> data_store;
};

#endif