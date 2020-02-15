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

size_t hash_custom(string) {

}