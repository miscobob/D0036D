#pragma once

#include "Player.h"

namespace client{
	Player::Player(int id, Colour colour) 
	{
		Player(id, colour, "");
	}

	Player::Player(int id, Colour colour, std::string name) 
	{
		this->id = id;
		this->colour = colour;
		this->name = name;
	}
	Colour Player::getColour() 
	{
		return colour;
	}

	int Player::getId() 
	{
		return id;
	}
	std::string Player::getName() 
	{
		return name;
	}
	void Player::setId(int id) 
	{
		if(id != 0 && id > 0)
			this->id = id;
		else 
		{
			printf("tried to set invalid id %d\n", id);
		}
	}
	void Player::setName(std::string name)
	{
		this->name = name;
	}
	bool Player::hasId() 
	{
		return id != 0;
	}

	void Player::setPos(int x, int y) 
	{
		std::pair<int, int> pos(x,y);
		this->pos = pos;
	}

	std::pair<int, int> Player::getPos() 
	{
		return pos;
	}
}