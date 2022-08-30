
#include "Network.h"
namespace client
{
	Network::Network() 
	{
		int res;
		res = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (res != 0) {
			printf("WSAStartup failed with error: %d\n", res);
			exit(EXIT_FAILURE);
		}
	}
	Network::~Network() 
	{
		close_sockets();
		WSACleanup();
	}
	int Network::setup(std::string server_ip)
	{
		addrinfo s_hints;
		addrinfo gui_sock_hints;
		int res;
		ZeroMemory(&s_hints, sizeof(s_hints));
		ZeroMemory(&gui_sock_hints, sizeof(gui_sock_hints));
		ZeroMemory(&gui, sizeof(gui));
		s_hints.ai_family = AF_INET;
		s_hints.ai_socktype = SOCK_STREAM;
		s_hints.ai_protocol = IPPROTO_TCP;
		gui_sock_hints.ai_family = AF_UNSPEC;
		gui_sock_hints.ai_socktype = SOCK_DGRAM;
		gui_sock_hints.ai_protocol = IPPROTO_UDP;
		gui.sin6_family = AF_INET6;
		gui.sin6_port = htons(45678);
		gui.sin6_addr = in6addr_loopback;
		gui.sin6_flowinfo = 0;

		

		
		PCSTR port = PCSTR("55226");
		PCSTR gui_sock_port = PCSTR("12345");
		//PCSTR gui_port = PCSTR("45678");
		PCSTR ip = PCSTR(server_ip.c_str());
		//PCSTR gui_sock_ip = ("0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1");
		/*
		res = getaddrinfo(ip, port, &s_hints, &this->server_addr);
		if (res != 0) {
			printf("getaddrinfo failed with error: %d\n", res);
			return -1;
		}
		*/
		res = getaddrinfo(NULL, gui_sock_port, &gui_sock_hints, &this->gui_addr);
		if (res != 0) {
			printf("getaddrinfo failed with error: %d\n", res);
			return -1;
		}

		/*
		this->ServerSocket = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
		if (this->ServerSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			//freeaddrinfo(server_addr);
			//freeaddrinfo(gui_addr);
			return -1;
		}*/
		this->GuiSocket = socket(gui_addr->ai_family, gui_addr->ai_socktype, gui_addr->ai_protocol);
		if (this->GuiSocket == INVALID_SOCKET) {
			printf("gui socket failed with error: %ld\n", WSAGetLastError());
			//freeaddrinfo(server_addr);
			//freeaddrinfo(gui_addr);
			return -1;
		}
		/*
		res = connect(this->ServerSocket, server_addr->ai_addr, (int)server_addr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			close_sockets();
			return -1;
		}*/
		res = bind(this->GuiSocket, gui_addr->ai_addr, (int)gui_addr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			printf("Failed to bind socket %d\n", WSAGetLastError());
			close_sockets();
			return -1;
		}
		return 0;
	}

	int Network::read_server_socket(Game *state) 
	{
		/*
		do poll
		*/
		char buf[BUFF_SIZE];
		int res = recv(ServerSocket, buf, BUFF_SIZE, 0);
		if (res < 0) 
		{
			printf("Failed to read socket %d\n", WSAGetLastError());
			return -1;
		}
		general::MsgHead* head;
		head = (general::MsgHead*)buf;
		//general::deserilize(head,buf_str);
		if (general::MsgType::Join == head->type)
		{
			general::JoinMsg* msg = (general::JoinMsg*)buf;
			/*send info to game state*/
		}
		else if (general::MsgType::Change == head->type) 
		{
			general::ChangeMsg* change_msg = (general::ChangeMsg*)buf;
			if (general::ChangeType::NewPlayer == change_msg->type) 
			{
				general::NewPlayerMsg* msg = (general::NewPlayerMsg*)buf;
			}
			else if (general::ChangeType::NewPlayerPosition == change_msg->type)
			{
				general::NewPlayerPositionMsg* msg = (general::NewPlayerPositionMsg*)buf;
			}
			else if (general::ChangeType::PlayerLeave == change_msg->type)
			{
				general::PlayerLeaveMsg* msg = (general::PlayerLeaveMsg*)buf;
			}
			else 
			{
				printf("Could not correctly interpret the change msg type");
				return -1;
			}
		}
		else if (general::MsgType::TextMessage == head->type)
		{
			general::TextMessageMsg* msg = (general::TextMessageMsg*)buf;
		}
		else
		{
			printf("buf: %s\n", buf);
			printf("No correct msg type was given %d\n", (int)head->type);
		}


	}

	int Network::send_to_server(char* msg, size_t size)
	{
		int res = send(ServerSocket, msg, size, 0);
		if (res == SOCKET_ERROR) {
			printf("Send to server failed with error: %d\n", WSAGetLastError());
			close_sockets();
			return -1;
		}
		return res;
	}

	int Network::send_to_gui(char* msg, size_t size)
	{
		int res = sendto(GuiSocket, msg, size, 0,(SOCKADDR *)&gui, sizeof(gui));
		
		if (res == SOCKET_ERROR) {
			printf("Send to gui failed with error: %d\n", WSAGetLastError());
			close_sockets();
			return -1;
		}
		return res;
	}

	void Network::close_sockets() 
	{
		closesocket(ServerSocket);
		closesocket(GuiSocket);
		GuiSocket = INVALID_SOCKET;
		ServerSocket = INVALID_SOCKET;
	}
}
