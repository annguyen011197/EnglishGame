#pragma once
#include <iostream>
#include <string>
class Player
{
	std::string ID;
	int Score;
public:
	Player();
	Player(std::string sID);
	std::string GetID();
	~Player();
};

