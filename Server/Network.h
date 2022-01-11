

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

namespace lab{
static const int MAX_CONNECTION = 10;
static const int TIME_OUT = 1000;
static const int BUFF_SIZE = 112;
class Network
{
private:

    static const int MAX_CONNECTION = 10;
    struct pollfd fd_read_set[MAX_CONNECTION+2]; // +2 one for end of array and one for the fd for listening socket
    struct pollfd fd_write_set[MAX_CONNECTION+1];
    struct sockaddr_in sa;
    int socket_fd;
    int nfds;

    /* data */
public:
    Network(/* args */);
    ~Network();
    void poll_fds();
    void quit(const char* message, int status);
};
}