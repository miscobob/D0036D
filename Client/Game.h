#pragma once

#include "../General/Protocol.h"
#include <string>
#include <list>
#include "Player.h"
#include "Network.h"
namespace client{
class Game
{
private:
	Player* self;
	int seq_no;
	std::map<int, Player *> otherPlayers;
	Network network;
	std::map<Colour, bool> selectedColours = {{BLUE, false}, {RED, false}, {GREEN, false}, {MAGENTA, false}, {ORANGE, false}, {PINK, false}, {WHITE, false}, {BLACK, false}, {CYAN, false}, {DARK_GRAY, false}};
public:
	Game(std::string serverIp, Colour selfColour, std::string name);
	~Game();
	Colour getColourSelf();
	void joinGame();
	void leaveGame();
	void moveToNewPosition(int x, int y);
	void handleJoinMsg(general::JoinMsg *msg);
	void handleNewPlayerMsg(general::NewPlayerMsg *msg);
	void handleNewPlayerPositionMsg(general::NewPlayerPositionMsg* msg);
	void handlePlayerLeaveMsg(general::PlayerLeaveMsg* msg);

};


}
