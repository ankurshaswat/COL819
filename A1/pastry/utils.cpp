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

// string ip_to_hash(string ip) {
//     hash<string> hasher;
//     size_t ip_hash_decimal = hasher(ip);
//     // cout<<ip_hash_decimal<<endl;

//     string hex_hash = "";

//     while(ip_hash_decimal>0) {
//         int next_digit = ip_hash_decimal%16;
//         ip_hash_decimal /= 16;

//         char next_char;

//         if(next_digit<10) {
//             next_char = '0' + next_digit;
//         } else {
//             next_char = 'A' + next_digit - 10;
//         }
//         // cout<<next_char<<endl;
//         hex_hash.insert(0,1,next_char);
//     }

//     while(hex_hash.length() < 32) {
//         // hex_hash.insert(0,"0");
//         hex_hash.append("0");
//     }

//     return hex_hash;
// }

string random_node_id(int len = 128)
{
    string s;
    for (int i = 0; i < len; i++)
    {
        int randomNum = rand();
        if (randomNum % 2 == 0)
        {
            s.append("0");
        }
        else
        {
            s.append("1");
        }
    }

    return s;
}

string binary_to_hex(string s)
{
    string hex_string;

    for (size_t i = 0; i < s.length(); i += 4)
    {
        string substr = s.substr(i, 4);

        if (substr == "0000")
        {
            hex_string.append("0");
        }
        else if (substr == "0001")
        {
            hex_string.append("1");
        }
        else if (substr == "0010")
        {
            hex_string.append("2");
        }
        else if (substr == "0011")
        {
            hex_string.append("3");
        }
        else if (substr == "0100")
        {
            hex_string.append("4");
        }
        else if (substr == "0101")
        {
            hex_string.append("5");
        }
        else if (substr == "0110")
        {
            hex_string.append("6");
        }
        else if (substr == "0111")
        {
            hex_string.append("7");
        }
        else if (substr == "1000")
        {
            hex_string.append("8");
        }
        else if (substr == "1001")
        {
            hex_string.append("9");
        }
        else if (substr == "1010")
        {
            hex_string.append("A");
        }
        else if (substr == "1011")
        {
            hex_string.append("B");
        }
        else if (substr == "1100")
        {
            hex_string.append("C");
        }
        else if (substr == "1101")
        {
            hex_string.append("D");
        }
        else if (substr == "1110")
        {
            hex_string.append("E");
        }
        else if (substr == "1111")
        {
            hex_string.append("F");
        }
    }

    return hex_string;
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

    // string common_prefix = s1.substr(0, common_len);

    // return common_prefix;
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