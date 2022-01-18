
#include "Network.h"
#include <stdexcept>
#include <fcntl.h>
#include <string>
#include "../General/Protocol.h"

namespace lab{
Network::Network(/* args */)
{
    this->socket_fd =  socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->socket_fd == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if(flags < 0)
    {
        perror("could not get flags");
        exit(EXIT_FAILURE);
    }
    flags = O_NONBLOCK & flags;
    int rs = fcntl(socket_fd,F_SETFL, flags);
    if(rs < 0)
    {
        perror("could not set flags");
        exit(EXIT_FAILURE);
    }
    //printf("1");
    memset(&this->sa, 0, sizeof(sa));
    this->sa.sin_family = AF_INET;
    this->sa.sin_port = htons(55226);
    inet_pton(AF_INET, "192.168.1.22", &(sa.sin_addr));
    //this->sa.sin_addr.s_addr = htonl(INADDR_ANY);
    //printf("2");
    if (bind(this->socket_fd,(struct sockaddr *)&this->sa, sizeof(this->sa)) == -1) {
        int errcode = errno;
        switch (errcode)
        {
        case EACCES:
            printf("The address is protected, and the user is not the superuser.");
            /* code */
            break;
        case EADDRINUSE:
            printf("The given address is already in use.");
            /* code */
            break;
        case EBADF:
            printf("sockfd is not a valid file descriptor.");
            /* code */
            break;
        case EINVAL:
            printf("The socket is already bound to a adress or addrlen is wrong, or addr is not a valid address for this socket's domain.");
            break;
        case ENOTSOCK:
            printf("The file descriptor sockfd does not refer to a socket."); 
            break;
        case EADDRNOTAVAIL:
            printf("Search permission is denied on a component on the path prefix");
            break;
        case EFAULT:
            printf("addr points outside the user's accessible adress space");
            break;
        case ELOOP:
            printf("Too many symbolic links were encountered in resolving addr");
            break;
        case ENAMETOOLONG:
            printf("addr is to long");
            break;
        case ENOENT:
            printf("A component is the directory prefix of the socket pathname does not exist.");
            break;
        case ENOMEM:
            printf("Insufficient kernel memory available.");
            break;
        case ENOTDIR:
            printf("A component of the path prefix is not a directory.");
            break;
        case EROFS:
            printf("The socket inode would reside on a read-only filesystem.");
            break;
        default:
            printf("Error code outside cases %d", errcode);
            break;
        }
        perror("\nbind failed");
        close(this->socket_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(this->socket_fd, 10) == -1) {
        perror("listen failed");
        close(this->socket_fd);
        exit(EXIT_FAILURE);
    }
    //printf("3");
    printf("accept fd %d\n",this->socket_fd);
    this->fd_read_set[0].fd = this->socket_fd;
    this->fd_read_set[0].events = POLLIN;
    this->nfds = 1;
    //printf("4");
    for(int i = 1; i<MAX_CONNECTION+1; i++)
    {
        this->fd_read_set[i].fd = -1;
        this->fd_read_set[i].events = POLLIN;
    }
    for(int i = 0; i<MAX_CONNECTION; i++)
    {
        this->fd_write_set[i].fd = -1;
        this->fd_write_set[i].events = POLLOUT;
    }
}


void Network::poll_fds()
{
    if(this->closed)
        return;
    int res;
    res = poll(this->fd_read_set, this->nfds, TIME_OUT);
    if(res == -1) {
        quit("poll failed", EXIT_FAILURE);
        //throw std::"poll failed";
    }
    else if (res == 0)
    {
        printf(" %d",this->fd_read_set[0].revents);
        printf(" timeout + nfds %d\n", this->nfds);
        return;
    }
    else
    {
        printf("res = %d\n", res);
        for(int i = 0; i<nfds; i++)
        {
            if(this->fd_read_set[i].revents == 0)
                continue;

            if((this->fd_read_set[i].revents&POLLIN == POLLIN )&&( this->fd_read_set[i].fd != -1))
            {
                if(this->fd_read_set[i].fd == this->socket_fd)
                {
                    int new_fd = accept(this->fd_read_set[i].fd, NULL, NULL);
                    if(new_fd == -1)
                    {
                        quit("accept failed", EXIT_FAILURE);
                        //throw "accept failed";
                    }
                    else
                    {
                        int complete = -1;
                        for(int i = 1; i<MAX_CONNECTION+1; i++)
                        {
                            
                            if(this->fd_read_set[i].fd == -1)
                            {
                                this->fd_read_set[i].fd = new_fd;
                                this->fd_write_set[i-1].fd = new_fd;
                                printf("Connection established %d\n",new_fd);
                                this->nfds = this->nfds + 1;
                                this->fd_read_set[i].revents = 0;//clears old revents
                                complete = 1;
                                break;
                            }
                        }
                        if(complete == -1)
                        {
                            printf("Connection list is full, dropped new connection\n");
                            close(new_fd);
                        }
                    }
                }
                else
                {
                    printf("waiting for read\n");
                    char buf[BUFF_SIZE];
                    res = read(this->fd_read_set[i].fd, buf, BUFF_SIZE);
                    if(res == -1)
                    {
                        printf("%d\n", i);
                        printf("read failed\n");
                        //throw "read failed";
                    }
                    else
                    {
                        int write_poll_res = poll(this->fd_write_set, this->nfds-1, 100);
                        if(write_poll_res == 0)
                        {
                            printf("Could not write a reply");
                        }
                        else if(write_poll_res < 0)
                        {
                            printf("poll failed");
                        }
                        else
                        {   
                            std::string name = "Example";
                                       
                            general::JoinMsg msg = general::get_join(name);
                            std::string rply = general::serilize(msg);
                            
                            for(int i = 0; i<nfds-1; i++)
                            {
                                if(fd_write_set[i].revents&POLLOUT)
                                {
                                    send(this->fd_write_set[i].fd, rply.c_str(),rply.size()+1, 0);
                                    printf("Reply sent to %d\n",this->fd_write_set[i].fd);
                                }
                            }
                            printf("done sending");
                            
                        }
                        printf("%.*s\n",(int)sizeof(buf),buf);
                    }
                }
            }
            //if(this->fd_read_set[i].revents&POLLRDHUP == POLLRDHUP)
            if(this->fd_read_set[i].revents&POLLRDHUP == POLLRDHUP && i != 0)
            {
                this->close_fd(i);
            }
        }
    }
    
}
void Network::close_fd(int fd_i)
{
    int closed_fd = this->fd_read_set[fd_i].fd;
    close(fd_read_set[fd_i].fd);
    fd_read_set[fd_i].fd = -1;
    fd_write_set[fd_i-1].fd = -1;
    this->nfds = this->nfds - 1;
    printf("Connection closed %d \n", closed_fd);
}

void Network::quit(const char* message, int status)
{
    printf("\n%s\n",message);
    for(int i = 0; i<MAX_CONNECTION+1; i++)
    {
        if(this->fd_read_set[i].fd != -1)
            close(this->fd_read_set[i].fd);
    }
    closed = true;
}

Network::~Network()
{
    this->quit("Destructor called", EXIT_SUCCESS);
}
}