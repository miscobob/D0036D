

#ifndef H_S_GAME
#define H_S_GAME

#include "../General/Protocol.h"
#include "Network.h"
#include "Player.h"
#include <algorithm>
#include <string>
#include <map>
#include <list>


namespace server
{

struct MsgToSend
{
    int fd;
    char* msg;
    size_t size;
};

class Game
{
private:
    Network net;
    bool run = false;
    std::list<MsgToSend> msg_queue;
    void remove_player(int fd);
public:
    //Map of all players key is the fd for that player
    std::map<int,Player*> players;
    Game(std::string ip);
    ~Game();
    void start();
    void player_join(int fd, general::JoinMsg* msg);
    void join_update(int fd, general::JoinMsg* msg);
    void player_leave(int fd);
    void new_player_pos(int fd, general::MoveEvent *msg);
    void quit(int status);
    void send_queued_msg();

};
    
}
#endif