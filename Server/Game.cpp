#include "Game.h"

namespace server
{

Game::Game()
{
    //this->net;
}
Game::~Game()
{

}
void Game::start()
{
    run = true;
    while(run)
    {
        net.poll_fds(this);
        send_queued_msg();
    }

}

void Game::remove_player(int fd)
{
    std::map<int, Player*>::iterator it;
    it = players.find(fd);
    players.erase(it);
}
void Game::player_join(int fd, general::JoinMsg* msg)
{
    general::JoinMsg reply;
    memcpy(&reply, msg, sizeof(reply));
    Player *p = new Player(std::string(msg->name));
    players.insert(std::pair<int,Player*>(fd, p));
    p->set_seq_no(1);
    reply.head.id = p->get_id();
    reply.head.seq_no = p->get_seq_no();
    MsgToSend msg_to_send;
    msg_to_send.fd = fd;
    memcpy(msg_to_send.msg, &reply, sizeof(reply));
    msg_to_send.size = sizeof(reply);
    msg_queue.push_back(msg_to_send);
    
}

void Game::join_update(int fd, general::JoinMsg* msg)
{
    
    for(std::map<int, Player*>::iterator it = players.begin(); it != players.end(); ++it)
    {
        if(it->first == fd)
            continue;
        Player *receiver = players[it->first];
        general::NewPlayerMsg update = general::get_newPlayer(msg->head.id, receiver->get_seq_no()+1, msg->desc, msg->form, std::string(msg->name));
        receiver->set_seq_no(receiver->get_seq_no()+1);
        MsgToSend msg_to_send;
        memcpy(msg_to_send.msg, &update, sizeof(update));
        msg_to_send.fd = it->first;
        msg_to_send.size = sizeof(update);
        msg_queue.push_back(msg_to_send);
    }
}
void Game::player_leave(int fd, general::LeaveMsg* msg)
{
    for(std::map<int, Player*>::iterator it = players.begin(); it != players.end(); ++it)
    {
        if(it->first == fd)
            continue;
        Player *receiver = players[it->first];
        general::PlayerLeaveMsg update = general::get_leavePlayer(msg->head.id, receiver->get_seq_no()+1);
        receiver->set_seq_no(receiver->get_seq_no()+1);
        MsgToSend msg_to_send;
        memcpy(msg_to_send.msg, &update, sizeof(update));
        msg_to_send.fd = it->first;
        msg_to_send.size = sizeof(update);
        msg_queue.push_back(msg_to_send);
    }
}

void Game::new_player_pos(int fd, general::MoveEvent* msg)
{
    for(std::map<int, Player*>::iterator it = players.begin(); it != players.end(); ++it)
    {
        if(it->first == fd)
            continue;
        Player *receiver = players[it->first];
        general::NewPlayerPositionMsg update = general::get_newPlayerPos(msg->event.head.id, receiver->get_seq_no()+1,msg->pos.x, msg->pos.y, msg->dir.x, msg->dir.y);
        receiver->set_seq_no(receiver->get_seq_no()+1);
        MsgToSend msg_to_send;
        memcpy(msg_to_send.msg, &update, sizeof(update));
        msg_to_send.fd = it->first;
        msg_to_send.size = sizeof(update);
        msg_queue.push_back(msg_to_send);
    }
}

void Game::send_queued_msg()
{
    int n_msg = this->msg_queue.size();
    printf("msg in queue: %d", n_msg);
    for(int i = 0; i <n_msg; i++)
    {
        MsgToSend msg = msg_queue.front();
        msg_queue.pop_front();
        int res = this->net.response(msg.msg, msg.size, msg.fd);
        if(res == -1)
        {
            printf("Error in send \n");
        }
        else if (res == 0)
        {
            printf("Msg back to queue\n");
            msg_queue.push_back(msg);
        }
    }
    
}

void Game::quit(int status){}
    
}  