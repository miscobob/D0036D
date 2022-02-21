
#ifndef Protocol_H
#define Protocol_H


#include <string.h>
#include <string>
#include <map>
namespace general
{
#define MAXNAMELEN 32
enum ObjectDesc
{
    Human,
    NonHuman,
    Vehicle,
    StaticObject
};
enum ObjectForm
{
    Cube,
    Sphere,
    Pyramid,
    Cone
};
struct Coordinate
{
    int x; 
    int y;
};
// Message head
enum MsgType
{
Join, // Client joining game at server
Leave, // Client leaving game
Change, // Information to clients
Event, // Information from clients to server
TextMessage // Send text messages to one or all
};
// Included first in all messages
struct MsgHead
{
unsigned int length;
unsigned int seq_no;
unsigned int id;
MsgType type;
};
// Message type Join (Client -> Server)  
 
struct JoinMsg 
{ 
MsgHead   head; 
ObjectDesc desc; 
ObjectForm form; 
char name[MAXNAMELEN];      // nullterminated!,  or empty
}; 
 
// Message type Leave (Client -> Server)  
 
 
struct LeaveMsg 
{ 
MsgHead  head; 
}; 
 
// Message type Change (Server -> Client)  
enum ChangeType 
{ 
NewPlayer, 
PlayerLeave, 
NewPlayerPosition 
}; 
// Included  first in all Change messages 
struct ChangeMsg 
{ 
MsgHead  head; 
ChangeType  type; 
}; 
 
 
// Variations of ChangeMsg  
 
struct NewPlayerMsg 
{ 
ChangeMsg  msg;  //Change  message header with new client id 
ObjectDesc  desc; 
ObjectForm  form; 
char  name[MAXNAMELEN];  // nullterminated!,  or empty 

};
 
struct PlayerLeaveMsg 
{ 
ChangeMsg  msg;  //Change  message header with new client id 
};

struct NewPlayerPositionMsg 
{ 
ChangeMsg  msg;  //Change  message header 
Coordinate  pos;  //New object position 
Coordinate  dir;  //New object direction (iqnored by game)
};
// Messages of type Event (Client ->  Server)  
 
enum EventType 
{ 
    Move 
}; 
// Included  first in all Event messages 
struct EventMsg 
{ 
    MsgHead  head; 
    EventType  type; 
}; 


// Variantions of EventMsg  

struct MoveEvent//
{ 
    EventMsg event;  
    Coordinate pos; //New object position 
    Coordinate dir; //New object direction 
}; 


// Messages of type TextMessage 
struct TextMessageMsg  // Optional  at client side!!! 
{ 
    MsgHead  head; 
    char  text[1];  // NULL-terminerad  array av chars. 
};
/**
 * Returns a head for msg sets length by defualt to 0 
 */
MsgHead get_head(int id, int seq_no, MsgType type);
/**
 * return a join msg with given name as name for the new player
 */
JoinMsg get_join(std::string name);
/**
 * return a leave msg for user with given ID,
 * seq_no is the returned msg sequence number
 */
LeaveMsg get_leave(int id, int seq_no);
/**
 * return a change msg for user with given id and of given change type,
 * seq_no is the returned msg sequence number
 */
ChangeMsg get_change(int id, int seq_no, ChangeType type);
/**
 * return a change msg for user with given id and of given event type,
 * seq_no is the returned msg sequence number
 */
EventMsg get_event(int id, int seq_no, EventType type);
/**
 * return a text msg for user with given id and text being the intended transfer text,
 * seq_no is the returned msg sequence number
 */
TextMessageMsg get_text(int id, int seq_no, std::string text);
/**
 * return a new player msg with given object description, form and player name,
 * id is the id of the inteded target of the msg,
 * seq_no is the returned msg sequence number
 */
NewPlayerMsg get_newPlayer(int id, int seq_no, ObjectDesc desc, ObjectForm form, std::string name);
/**
 * return a player leave msg,
 * id is the id of the inteded target of the msg,
 * seq_no is the returned msg sequence number
 */
PlayerLeaveMsg get_leavePlayer(int id, int seq_no);
/**
 * return a new player position msg
 * x,y is the new position v_x, v_y is the direction
 * id is the id of the inteded target of the msg,
 * seq_no is the returned msg sequence number
 */
NewPlayerPositionMsg get_newPlayerPos(int id, int seq_no, int x, int y, int v_x, int v_y);
}
#endif