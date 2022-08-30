
#include "Network.h"
#include <iostream>

int main(int argc, char* argv[])
{
    client::Network* net;
    net = new client::Network();
    int res;
    if (argc == 2)
    {
        std::string ip(argv[1]);
        res = net->setup(ip);
    }
    else
    {
        std::string ip("192.168.1.15");
        res = net->setup(ip);
    }
    if (res == -1) 
    {
        printf("Failed to create socket");
        exit(EXIT_FAILURE);
    }
    std::string n = "{198,198,magenta}";
    char * test = new char[n.size()+1];
    std::copy(n.begin(), n.end(), test);
    test[n.size()] = '\0';
    printf(test);
    net->send_to_gui(test,strlen(test));
    std::cin >> n;

}