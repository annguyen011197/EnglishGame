#include "stdafx.h"
#include "PlayerManager.h"


bool PlayerManager::AddNewPlayer(CString ID)
{
	Player * newPlayer = new Player(ID);
	PlayerList.push_back(newPlayer);
	return true;
}

bool PlayerManager::CheckID(CString ID)
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

void PlayerManager::DeletePlayer(int ID)
{
	delete this->PlayerList[ID];
	this->PlayerList.erase(PlayerList.begin() + ID);
}

int PlayerManager::getSize()
{
	return PlayerList.size();
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
