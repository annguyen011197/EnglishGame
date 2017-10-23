#pragma once
#include <string>
#include "resource.h"
using namespace std;
class Message
{
	int m_Message;
	CString info;
public:
	Message();
	Message(int nMess, CString sInfo);
	Message(Message &mess);
	void SetNewMessage(int nMess, CString sInfo);
	int GetMess();
	CString GetInfo();
	~Message();
};

