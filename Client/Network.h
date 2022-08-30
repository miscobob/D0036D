
#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../General/Protocol.h"
#include "Game.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_PORT 55226
#define GUI_PORT 98765

namespace client 
{
static const int BUFF_SIZE = 112;

class Network 
{
private:
	WSADATA wsaData;
	SOCKET ServerSocket = INVALID_SOCKET;
	SOCKET GuiSocket = INVALID_SOCKET;
	addrinfo *server_addr = NULL;
	addrinfo *gui_addr = NULL;
	sockaddr_in6 gui;

public:
	Network();
	~Network();
	int setup(std::string server_ip);
	int read_server_socket(Game* state);
	int send_to_server(char* msg, size_t size);
	int send_to_gui(char* msg, size_t size);
	void close_sockets();
};

}