#pragma once
#include "Player.h"
#include <vector>
#include <string>
class PlayerManager
{
	std::vector<Player*> PlayerList;
public:
	bool AddNewPlayer(CString ID);
	bool CheckID(CString ID);
	void DeletePlayer(int ID);
	int getSize();
	PlayerManager();
	~PlayerManager();
};

