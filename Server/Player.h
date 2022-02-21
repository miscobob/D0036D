
#ifndef H_S_PLAYER
#define H_S_PLAYER

#include <string>
#include <map>
#include <list>

namespace server{
    
class Player
{
private:
    int id = 0;
    int seq_no = 0;
    std::string name;
public:
    Player(std::string name);
    ~Player();
    int get_id();
    int get_seq_no();
    void set_seq_no(int next);
    std::string get_name();
};


}
#endif