#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

string get_random_ip();
string random_node_id(int);
string binary_to_hex(string);
int get_common_prefix_len(string, string);
string find_closest_node_util(string, vector<string>);
// string ip_to_hash(string);
string get_dist(string s1, string s2);
char int_to_hex(int n);
int hex_to_int(char c);
string upper_md5(string);
string generate_random_string(int);

struct msg_type
{
    string type;
    string target_id;
    string sender_id;
    string key;
    string value;
};

#endif