#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

struct msg_type
{
    string type;
    size_t target_id;
    size_t sender_id;
    size_t key;
    string value;
};

string get_random_ip();
size_t hash_custom(string);
string generate_random_string(int);
bool in_range(size_t target, size_t lower_lim, size_t upper_lim);
size_t circular_subtract(size_t a, size_t b);

#endif