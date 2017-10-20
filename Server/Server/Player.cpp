#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	ID = "";
	Score = -1;
}

Player::Player(std::string sID)
{
	ID = sID;
	Score = 0;
}

std::string Player::GetID()
{
	return this->ID;
}


Player::~Player()
{
}
