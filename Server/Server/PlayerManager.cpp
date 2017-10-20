#include "stdafx.h"
#include "PlayerManager.h"


bool PlayerManager::AddNewPlayer(std::string ID)
{
	Player * newPlayer = new Player(ID);
	PlayerList.push_back(newPlayer);
	return true;
}

bool PlayerManager::CheckID(std::string ID)
{
	if (PlayerList.size() == 0) {
		return true;
	}
	for (int i = 0; i < PlayerList.size(); ++i) {
		if (ID == PlayerList[i]->GetID()) {
			return false;
		}
	}
	return true;
}

PlayerManager::PlayerManager()
{
}


PlayerManager::~PlayerManager()
{
	for (auto i : PlayerList) {
		delete i;
	}
	PlayerList.resize(0);
}
