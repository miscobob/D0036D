

#ifndef H_S_NET
#define H_S_NET


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>



namespace server{

class Game;

static const int MAX_CONNECTION = 10;
static const int TIME_OUT = 1000;
static const int BUFF_SIZE = 112;
class Network
{
private:

    static const int MAX_CONNECTION = 10;
    struct pollfd fd_read_set[MAX_CONNECTION+2]; // +2 one for end of array and one for the fd for listening socket
    struct sockaddr_in sa;
    int socket_fd;
    int nfds;
    bool closed = false;
    void close_fd(int fd_i);

    /* data */
public:
    Network();
    ~Network();
    void poll_fds(Game *state);
    void quit(const char* message, int status);
    /**
    * Takes a char pointer to a message, the size of the message
    * and fd of target msg returns int indecation of success, -1, 0 or 1 
    * -1 failure network closed or error occoured in poll,
    *  0 could not send msg, 1 msg sent
    */
    int response(char *msg, size_t size, int fd);
};
}
#endif