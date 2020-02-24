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

using namespace std;

#define NUM_DIGITS 8
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

int get_common_prefix_len(string s1, string s2)
{

    int common_len = 0;

    for (size_t i = 0; i < s1.length() && i < s2.length(); i++)
    {
        if (s1[i] == s2[i])
        {
            common_len++;
        }
        else
        {
            break;
        }
    }

    return common_len;
}

string find_closest_node_util(string target, vector<string> nodes)
{
    string closest_node = "";
    string lowest_hex_distance(NUM_DIGITS, 'F');
    vector<string>::iterator itr;

    for (itr = nodes.begin(); itr != nodes.end(); ++itr)
    {
        if (*itr == "")
        {
            continue;
        }
        string distance = get_dist(target, *itr);
        if (distance < lowest_hex_distance)
        {
            closest_node = *itr;
            lowest_hex_distance = distance;
        }
    }

    return closest_node;
}

int hex_to_int(char c)
{
    if (c <= '9')
    {
        return c - '0';
    }
    else
    {
        return c - 'A' + 10;
    }
}

char int_to_hex(int n)
{
    if (n < 10)
    {
        return n + '0';
    }
    else
    {
        return n - 10 + 'A';
    }
}

string get_dist(string s1, string s2)
{
    string greater, smaller;

    string res(NUM_DIGITS, '0');

    if (s1 < s2)
    {
        smaller = s1;
        greater = s2;
    }
    else
    {
        smaller = s2;
        greater = s1;
    }

    int carry = 0;

    for (int i = NUM_DIGITS - 1; i >= 0; i--)
    {
        if (hex_to_int(greater[i]) + carry < hex_to_int(smaller[i]))
        {
            res[i] = int_to_hex(16 + hex_to_int(greater[i]) - hex_to_int(smaller[i]) + carry);
            carry = -1;
        }
        else
        {
            res[i] = int_to_hex(hex_to_int(greater[i]) - hex_to_int(smaller[i]) + carry);
            carry = 0;
        }
    }

    return res;
}

string upper_md5(string inp)
{
    size_t string_hash = hash<string>{}(inp);
    string_hash = string_hash % limit;

    string final = "";

    while (string_hash > 0)
    {
        int next_digit = string_hash % 16;
        string_hash /= 16;
        final.push_back(int_to_hex(next_digit));
    }

    while (final.length() < NUM_DIGITS)
    {
        final.push_back('0');
    }

    reverse(final.begin(), final.end());

    // string res = md5(inp);
    // transform(res.begin(), res.end(), res.begin(), ::toupper);
    // return res;
    return final;
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
        // s.append(conv_to_string(char(rand() % 128)));
    }

    return s;
}