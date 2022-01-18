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
        strcpy(msg.name, name.c_str());
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
        strcpy(msg.text, text.c_str());
        return msg;
    }
    NewPlayerMsg get_newPlayer(int id, int seq_no, ObjectDesc desc, ObjectForm form, std::string name)
    {
        NewPlayerMsg msg;
        msg.msg = get_change(id, seq_no, ChangeType::NewPlayer);
        msg.desc = desc;
        msg.form = form;
        strcpy(msg.name, name.c_str());
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


    std::string serilize(MsgHead head)
    {
        std::string res = "length=|";
        res.append(std::to_string(head.length));
        res.append("|seq_no=|");
        res.append(std::to_string(head.seq_no));
        res.append("|id=|");
        res.append(std::to_string(head.id));
        int t = (int)head.type;
        res.append("|type=|");
        res.append(std::to_string(t));
        res.append("|");
        return res;
    }

    std::string serilize(JoinMsg msg)
    {
        std::string res_str = serilize(msg.head);
        res_str.append("desc=|");
        int t = (int)msg.desc;
        res_str.append(std::to_string(t));
        res_str.append("|form=|");
        t = (int)msg.form;
        res_str.append(std::to_string(t));
        res_str.append("|name=|");
        res_str.append(msg.name);
        res_str.append("|");
        return res_str;

    }
    std::string serilize(LeaveMsg msg)
    {
        return  serilize(msg.head);
    }
    std::string serilize(ChangeMsg msg)
    {
        std::string res_str = serilize(msg.head);
        int t = (int)msg.type;
        res_str.append("change_type=|");
        res_str.append(std::to_string(t));
        res_str.append("|");
        return res_str;
    }
    std::string serilize(EventMsg msg)
    {
        std::string res_str = serilize(msg.head);
        int t = (int)msg.type;
        res_str.append("event_type=|");
        res_str.append(std::to_string(t));
        res_str.append("|");
        return res_str;
    }
    std::string serilize(TextMessageMsg  msg)
    {
        std::string res_str = serilize(msg.head);
        res_str.append("text=|");
        res_str.append(msg.text);
        res_str.append("|");
        return res_str;
    }
    std::string serilize(NewPlayerMsg msg)
    {
        std::string res_str  = serilize(msg.msg);
        res_str.append("desc=|");
        int t = (int)msg.desc;
        res_str.append(std::to_string(t));
        res_str.append("|form=|");
        t = (int)msg.form;
        res_str.append(std::to_string(t));
        res_str.append("|name=|");
        res_str.append(msg.name);
        res_str.append("|");
        return res_str;
    }
    std::string serilize(PlayerLeaveMsg msg)
    {
        return serilize(msg.msg);
    }
    std::string serilize(NewPlayerPositionMsg msg)
    {
        std::string res_str  = serilize(msg.msg);
        res_str.append("pos=|");
        res_str.append(serilize(msg.pos));
        res_str.append("|dir=|");
        res_str.append(serilize(msg.dir));
        res_str.append("|");
        return res_str;

    }
    std::string serilize(MoveEvent event)
    {
        std::string res_str  = serilize(event.event);
        res_str.append("pos=|");
        res_str.append(serilize(event.pos));
        res_str.append("|dir=|");
        res_str.append(serilize(event.dir));
        res_str.append("|");
        return res_str;

    }
    std::string serilize(Coordinate cord)
    {
        std::string res_str = "x=:";
        res_str.append(std::to_string(cord.x));
        res_str.append(":y=:");
        res_str.append(std::to_string(cord.y));
        res_str.append(":");
        return res_str;
    }
    
    //

    void deserilize(MsgHead &head, std::string &msg)
    {
        std::string tag = "length=";
        std::string length = retrive_value(msg, tag);
        if(length.empty())
            return;
        tag.clear();
        tag.append("seq_no=");
        std::string seq_no = retrive_value(msg, tag);
        if(seq_no.empty())
            return;
        tag.clear();
        tag.append("id=");
        std::string id = retrive_value(msg, tag);
        if(id.empty())
            return;
        tag.clear();
        tag.append("type=");
        std::string type = retrive_value(msg, tag);
        if(type.empty())
            return;
        head.length= std::stoi(length);
        head.seq_no = std::stoi(seq_no);
        head.id = std::stoi(id);
        head.type = (MsgType)std::stoi(type);
        
    }
    void deserilize(JoinMsg &join, std::string &msg)
    {
        std::string tag = "desc=";
        std::string desc = retrive_value(msg, tag);
        if(desc.empty())
            return;
        tag.clear();
        tag.append("form=");
        std::string form = retrive_value(msg, tag);
        if(form.empty())
            return;
        tag.clear();
        tag.append("name=");
        std::string name = retrive_value(msg, tag);
        if(name.empty())
            return;
        join.desc = (ObjectDesc)std::stoi(desc);  
        join.form = (ObjectForm)std::stoi(form);       
        strcpy(join.name,name.c_str());
    }
    void deserilize(ChangeMsg &change,std::string &msg)
    {
        std::string tag = "type=";
        std::string type = retrive_value(msg, tag);
        if(type.empty())
            return;
        change.type = (ChangeType)std::stoi(type);
    }
    void deserilize(EventMsg &event,std::string &msg)
    {
        std::string tag = "type=";
        std::string type = retrive_value(msg, tag);
        if(type.empty())
            return;
        event.type = (EventType)std::stoi(type);    
    }
    void deserilize(TextMessageMsg &text, std::string  &msg)
    {
        std::string tag = "text=";
        std::string textv = retrive_value(msg, tag);
        if(textv.empty())
            return;
        strcpy(text.text, textv.c_str());
    }
    void deserilize(NewPlayerMsg &new_player, std::string &msg)
    {
        std::string tag = "desc=";
        std::string desc = retrive_value(msg, tag);
        if(desc.empty())
            return;
        tag.clear();
        tag.append("form=");
        std::string form = retrive_value(msg, tag);
        if(form.empty())
            return;
        tag.clear();
        tag.append("name=");
        std::string name = retrive_value(msg, tag);
        if(name.empty())
            return;
        new_player.desc = (ObjectDesc)std::stoi(desc);  
        new_player.form = (ObjectForm)std::stoi(form);       
        strcpy(new_player.name,name.c_str());
    }
    void deserilize(NewPlayerPositionMsg &player_pos,std::string &msg)
    {
        std::string tag = "pos=";
        std::string pos = retrive_value(msg, tag);
        if(pos.empty())
            return;
        tag.clear();
        tag.append("dir=");
        std::string dir = retrive_value(msg, tag);
        if(dir.empty())
            return;
        deserilize(player_pos.pos, pos);
        deserilize(player_pos.dir, dir);
    }
    void deserilize(MoveEvent &move_event, std::string &msg)
    {
        std::string tag = "pos=";
        std::string pos = retrive_value(msg, tag);
        if(pos.empty())
            return;
        tag.clear();
        tag.append("dir=");
        std::string dir = retrive_value(msg, tag);
        if(dir.empty())
            return;
        deserilize(move_event.pos, pos);
        deserilize(move_event.dir, dir);
    }

    void deserilize(Coordinate &coord, std::string &msg)
    {
        std::replace(msg.begin(),msg.end(), ':', '|');
        std::string tag = "x=";
        std::string x = retrive_value(msg, tag);
        if(x.empty())
            return;
        tag.clear();
        tag.append("y=");
        std::string y = retrive_value(msg, tag);
        if(y.empty());
            return;
        coord.x = std::stoi(x);
        coord.y = std::stoi(y);
    }

    std::string retrive_value(std::string &msg, std::string tag)
    {
        std::size_t found = msg.find(tag);
        if(found == std::string::npos)
        {
            std::cout<<"failed to find tag "<< tag <<" in message \n";
            std::cout<< msg<< "\n";
            return "";
        }
        msg.erase(0,(int)found);
        while(true){
            if((msg.front()) == '|')
            {
                msg.erase(0,1);
                break;
            }
            msg.erase(0,1);
        }
        std::string value = "";
        while(true){
            if((msg.front()) == '|')
            {
                msg.erase(0,1);
                break;
            }
            value.push_back(msg.front());
            msg.erase(0,1);
        }
        return value;
    }
}