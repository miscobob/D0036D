
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
Coordinate  dir;  //New object direction 
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

struct MoveEvent 
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

JoinMsg get_join(char *name);
}