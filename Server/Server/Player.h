#pragma once
#include <iostream>
#include <string>
class Player
{
	CString ID;
	int Score;
public:
	Player();
	Player(CString sID);
	CString GetID();
	~Player();
};

