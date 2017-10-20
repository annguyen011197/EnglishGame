#pragma once
#include "Player.h"
#include <vector>
#include <string>
class PlayerManager
{
	std::vector<Player*> PlayerList;
public:
	bool AddNewPlayer(std::string ID);
	bool CheckID(std::string ID);
	PlayerManager();
	~PlayerManager();
};

