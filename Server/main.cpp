
#include "Network.h"
#include "../General/Protocol.h"
#include <iostream>
int main(void)
{
    char name[32];
    std::cin.read(name, 4);
    general::JoinMsg msg = general::get_join(name);
    printf("%s",msg);
    //lab::Network* net = new lab::Network();
    /*while(true){
        //printf("New roud\n");
        net->poll_fds();
    }*/
    
}

