
#include "Game.h"
#include "../General/Protocol.h"
#include <iostream>
#include <iomanip>

namespace client{
	Game::Game(std::string serverIp,Colour selfColour, std::string name)
	{
		network.setup(serverIp);
		self = new Player(0,selfColour, name);
		selectedColours.find(selfColour)->second = true;
	}

	Game::~Game() 
	{
		delete self;
		while (!otherPlayers.empty()) delete otherPlayers.begin()->second, otherPlayers.erase(otherPlayers.begin());
		network.close_sockets();
	}
	Colour Game::getColourSelf()
	{
		return self->getColour();
	}
	void Game::joinGame() 
	{
		if (self->hasId()) 
		{
			std::cout << "Enter name:\n";
			std::string name;
			std::cin >> std::setw(32) >> name;
			general::JoinMsg msg = general::get_join(name);
		}
		else 
		{
			printf("Already has joined the game");
		}
	}
	void Game::handleJoinMsg(general::JoinMsg* msg)
	{
		seq_no = msg->head.seq_no + 1;
		if(!self->hasId())
			self->setId(msg->head.id);
		else
		{
			printf("Got join msg even though client already has joined match\n");
		}
	}

	void Game::handleNewPlayerMsg(general::NewPlayerMsg* msg)
	{
		int id = msg->msg.head.id;
		seq_no = msg->msg.head.seq_no + 1;
		std::string name(msg->name);
		std::map<Colour, bool>::iterator it = selectedColours.begin();
		bool selectColour = false;
		Colour c;
		while (!selectColour) 
		{
			if (it == selectedColours.end()) 
			{
				printf("Could not give colour to player");
				leaveGame();
			}
			else if (!it->second) 
			{
				c = it->first;
				it->second = true;
				selectColour = true;
			}
		}
		Player * p = new Player(id, c, name);
		otherPlayers.emplace(id, p);
		printf("new player joined %s", p->getName());
	}

	void Game::handleNewPlayerPositionMsg(general::NewPlayerPositionMsg* msg)
	{
		
		int id = msg->msg.head.id;
		seq_no = msg->msg.head.seq_no + 1;
		if (id == self->getId()) 
		{

		}
		else 
		{
			Player* p = otherPlayers[id];
			std::pair<int, int> currentPos = p->getPos();
			p->setPos(msg->dir.x, msg->dir.y);
			std::string colour = colourTable[p->getColour()];
			std::string guiMsg("{" + std::to_string(msg->dir.x)+","+ std::to_string(msg->dir.x)+","+ colour+ "}");
			char* sendData = new char[guiMsg.size()+1];
			std::copy(guiMsg.begin(), guiMsg.end(), sendData);
			sendData[guiMsg.size()] = '\0';
			network.send_to_gui(sendData,guiMsg.length());
			delete sendData;
		}

	}

	void Game::handlePlayerLeaveMsg(general::PlayerLeaveMsg* msg)
	{
		int id = msg->msg.head.id;
		seq_no = msg->msg.head.seq_no + 1;
		if (otherPlayers.find(id) == otherPlayers.end()) 
		{
			printf("Player with ID %d left the game but could not be found in map", id);
		}
		else 
		{
			otherPlayers.erase(id);
			printf("Player with ID %d left the game", id);
		}
	}

	void Game::leaveGame()
	{
		general::LeaveMsg msg = general::get_leave(self->getId(), seq_no);
		char* data;
		memcpy(data, &msg, sizeof(msg));
		this->network.send_to_server(data, sizeof(msg));

		this->network.close_sockets();
	}

}