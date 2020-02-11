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
    // for (int i = 0; i < 10; i++)
    // {
    //     string random_ip = get_random_ip();
    //     cout << "Random IP = " << random_ip << endl;
    //     cout << "IP Hash = " << ip_to_hash(random_ip) << endl;
    // }

    // cout<<"9<A "<<("9"<"A")<<endl;

    coordinator* coord = new coordinator(200000);
    coord->start_simulation();

    return 0;
}