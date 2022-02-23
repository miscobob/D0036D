
#include "Game.h"
#include "../General/Protocol.h"
#include <iostream>
#include <string>

int main(int argc, char * argv[])
{
    if(argc == 2)
    {
        std::string ip(argv[1]);
        server::Game game(ip);
        game.start();
    }
    else
    {
        std::string ip("192.168.1.15");
        server::Game game(ip);
    }
    
}

