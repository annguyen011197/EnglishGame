#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	ID = "";
	Score = -1;
}

Player::Player(CString sID)
{
	ID = sID;
	Score = 0;
}

CString Player::GetID()
{
	return this->ID;
}


Player::~Player()
{
}
