
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
/**
 * Takes the head of msg and return a string representation
 */
std::string serilize(MsgHead head);
/**
 * Takes a join msg and return a string representation
 */
std::string serilize(JoinMsg msg);
/**
 * Takes a leave msg and return a string representation
 */
std::string serilize(LeaveMsg msg);
/**
 * Takes a change msg and return a string representation
 */
std::string serilize(ChangeMsg msg);
/**
 * Takes a event msg and return a string representation
 */
std::string serilize(EventMsg msg);
/**
 * Takes a text msg and return a string representation
 */
std::string serilize(TextMessageMsg  msg);
/**
 * Takes a new player msg and return a string representation
 */
std::string serilize(NewPlayerMsg msg);
/**
 * Takes a join msg and return a string representation
 */
std::string serilize(PlayerLeaveMsg msg);
/**
 * Takes a join msg and return a string representation
 */
std::string serilize(NewPlayerPositionMsg msg);
/**
 * Takes a join msg and return a string representation
 */
std::string serilize(MoveEvent event);
/**
 * Takes a join msg and return a string representation
 */
std::string serilize(Coordinate cord);
//
/**
 * Takes a msg string and returns head of msg. 
 * WARNING MODIFIES STRING.
 */
MsgHead deserilize(std::string &msg);
/**
 * Takes a msg string and a msg head and returns a join msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that msg head type is correct
 */
JoinMsg deserilize(MsgHead msg_head, std::string &msg);
/**
 * Takes a msg string and a msg head and returns a leave msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that msg head type is correct
 */
LeaveMsg deserilize(MsgHead msg_head,std::string &msg);
/**
 * Takes a msg string and a msg head and returns a change msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that msg head type is correct
 */
ChangeMsg deserilize(MsgHead msg_head,std::string &msg);
/**
 * Takes a msg string and a msg head and returns a event msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that msg head type is correct
 */
EventMsg deserilize(MsgHead msg_head,std::string &msg);
/**
 * Takes a msg string and a msg head and returns a text msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that msg head type is correct
 */
TextMessageMsg deserilize(MsgHead msg_head, std::string &msg);
/**
 * Takes a msg string and a change msg and returns a new player msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that change type is correct
 */
NewPlayerMsg deserilize(ChangeMsg change_msg, std::string &msg);
/**
 * Takes a msg string and a change msg and returns a player leave msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that change type is correct
 */
PlayerLeaveMsg deserilize(ChangeMsg change_msg,std::string &msg);
/**
 * Takes a msg string and a change msg and returns a new player position msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that change type is correct
 */
NewPlayerPositionMsg deserilize(ChangeMsg change_msg,std::string &msg);
/**
 * Takes a msg string and a evnet msg and returns a move event msg. 
 * WARNING MODIFIES STRING.
 * Does not verify that event type is correct
 */
MoveEvent deserilize(EventMsg event_msg, std::string &msg);
/**
 * takes a msg and returns first set of coordinates
 * * WARNING MODIFIES STRING.
 */
Coordinate deserilize(std::string &msg);
/**
 * returns value of given tag in msg string and remove all 
 * from begnning of string up to the end marker for the taged value
 */
std::string retrive_value(std::string &msg, std::string tag);
}
#endif