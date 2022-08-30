#include "Protocol.h"
#include <iterator>
#include <iostream>
#include <algorithm>
namespace general{
    MsgHead get_head(int id, int seq_no, MsgType type)
    {
        MsgHead head;
        head.id = id;
        head.type = type;
        head.length = 0; // just setting it to zero
        return head;
    }
    JoinMsg get_join(std::string name)
    {
        JoinMsg msg;
        msg.head = get_head(0,0, MsgType::Join);
        strcpy_s(msg.name, name.c_str());
        msg.desc = ObjectDesc::Human;
        msg.form = ObjectForm::Cube;
        msg.head.length = sizeof(msg);
        return msg;
    }
    LeaveMsg get_leave(int id, int seq_no)
    {
        LeaveMsg msg;
        msg.head = get_head(id, seq_no, MsgType::Leave);
        return msg;
    }
    ChangeMsg get_change(int id, int seq_no, ChangeType type)
    {
        ChangeMsg msg;
        msg.head = get_head(id, seq_no, MsgType::Change);
        msg.type = type;
        return msg;
    }
    
    EventMsg get_event(int id, int seq_no, EventType type)
    {
        EventMsg msg;
        msg.head = get_head(id, seq_no, MsgType::Event);
        msg.type = type;
        return msg;
    }

    TextMessageMsg get_text(int id, int seq_no, std::string text)
    {
        TextMessageMsg msg;
        msg.head = get_head(id, seq_no, MsgType::TextMessage);
        strcpy_s(msg.text, text.c_str());
        return msg;
    }

    NewPlayerMsg get_newPlayer(int id, int seq_no, ObjectDesc desc, ObjectForm form, std::string name)
    {
        NewPlayerMsg msg;
        msg.msg = get_change(id, seq_no, ChangeType::NewPlayer);
        msg.desc = desc;
        msg.form = form;
        strcpy_s(msg.name, name.c_str());
        msg.msg.head.length = sizeof(msg);
        return msg;

    }
    PlayerLeaveMsg get_leavePlayer(int id, int seq_no)
    {
        PlayerLeaveMsg msg;
        msg.msg = get_change(id, seq_no, ChangeType::PlayerLeave);
        msg.msg.head.length = sizeof(msg);
        return msg;
    }
    NewPlayerPositionMsg get_newPlayerPos(int id, int seq_no, int x, int y, int v_x, int v_y)
    {
        NewPlayerPositionMsg msg;
        msg.msg = get_change(id, seq_no, ChangeType::NewPlayerPosition);
        msg.pos.x = x;
        msg.pos.y = y;
        msg.dir.x = v_x;
        msg.dir.y = v_y;
        msg.msg.head.length = sizeof(msg);
        return msg;
    }

}