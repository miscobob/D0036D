#include "Protocol.h"
#include <iterator>
#include <string.h>
namespace general{
    JoinMsg get_join(char* name)
    {
        JoinMsg msg;
        msg.head.id = 0;
        msg.head.seq_no = 0;
        msg.head.type = MsgType::Join;
        strncpy(msg.name, name, 32);
        msg.desc = ObjectDesc::Human;
        msg.form = ObjectForm::Cube;
        msg.head.length = sizeof(msg);
        return msg;
    }
}