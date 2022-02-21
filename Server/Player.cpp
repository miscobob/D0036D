
#include "Player.h"
#include <stdlib.h>
#include <algorithm>
namespace server
{


std::list<int> id_list;
Player::Player(std::string name)
{
    this->name = name;
    int id = 0;
    while(!id) 
    {
        id = rand()+1;
        std::list<int>::iterator it = std::find(id_list.begin(), id_list.end(), id);
        if(*it == id)
        {
            id = 0;
        }
    }
    id_list.push_back(id);
    this->id = id;
    //std::list<int>::iterator it = 
    printf("id = %d\n", id);
}

void Player::set_seq_no(int next)
{
    this->seq_no = next;
}

Player::~Player()
{
    std::list<int>::iterator it = std::find(id_list.begin(), id_list.end(), this->id);
    id_list.erase(it);
}

int Player::get_id()
{
    return this->id;
}
int Player::get_seq_no()
{
    return this->seq_no;
}
    
} // namespace server