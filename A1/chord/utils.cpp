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
#include <algorithm>
#include <string>
#include <math.h>

using namespace std;

// #define limit 16U
#define limit 4294967296U

string get_random_ip()
{
    string s;
    for (int i = 0; i < 4; i++)
    {
        s.append(to_string(rand() & 0xFF));
        if (i != 3)
        {
            s.append(".");
        }
    }

    return s;
}

size_t hash_custom(string node_ip)
{
    size_t string_hash = hash<string>{}(node_ip);
    return string_hash % limit;
}

string conv_to_string(char c)
{
    string s(1, c);
    return s;
}

string generate_random_string(int len)
{
    string char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-+!@#$%^&*()_+{}:";
    string s;

    for (int i = 0; i < len; i++)
    {
        char c = char_set[rand() % char_set.size()];
        s.append(conv_to_string(c));
    }

    return s;
}

bool in_range(size_t target, size_t lower_lim, size_t upper_lim)
{
    // DEBUG(lower_lim << "<" << target << "<" << upper_lim);
    if (lower_lim == upper_lim)
    {
        // DEBUG("False");
        return false;
    }
    if (lower_lim < upper_lim)
    {
        // Normal Case
        // DEBUG((lower_lim < target && target < upper_lim));

        return (lower_lim < target && target < upper_lim);
    }
    else
    {
        // Lower lim to (0-1)
        if (lower_lim < target && target < limit)
        {
            // DEBUG("true");

            return true;
        }

        // 0 to upper lim
        if (target < upper_lim) //0 <= target always true for unsigned
        {
            // DEBUG("true");

            return true;
        }

        // DEBUG("False");

        return false;
    }
}

size_t circular_subtract(size_t a, size_t b)
{
    // DEBUG(a << " " << b);

    if (a >= b)
    {
        DEBUG((a - b));
        return a - b;
    }
    else
    {
        // DEBUG(((a + (limit - b)) % limit));

        return (a + (limit - b)) % limit;
    }
}