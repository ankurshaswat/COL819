#ifndef _DEBUG
#define DEBUG(x) 
#else
#define DEBUG(x) do { std::cerr << x << std::endl; } while (0)
#endif

#include <utils.h>
#include <iostream>
#include <coordinator.h>

using namespace std;

int main()
{
    coordinator* coord = new coordinator(200000);
    coord->start_simulation();

    return 0;
}