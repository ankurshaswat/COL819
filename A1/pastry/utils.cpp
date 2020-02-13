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
#include <md5.h>

using namespace std;

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
    string lowest_hex_distance = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
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

    string res = "00000000000000000000000000000000";

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

    for (int i = 31; i >= 0; i--)
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

string upper_md5(string inp) {
    string res = md5(inp);
    transform(res.begin(), res.end(), res.begin(), ::toupper); 
    return res;
}