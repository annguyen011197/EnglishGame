#include "stdafx.h"
#include "Message.h"


Message::Message()
{
	m_Message = 0;
	info = "";
}

Message::Message(int nMess, CString sInfo)
{
	m_Message = nMess;
	info = sInfo;
}

Message::Message(Message & mess)
{
	m_Message = mess.GetMess();
	info = mess.GetInfo();
}

void Message::SetNewMessage(int nMess, CString sInfo)
{
	m_Message = nMess;
	info = sInfo;
}

int Message::GetMess()
{
	return m_Message;
}

CString Message::GetInfo()
{
	return info;
}



Message::~Message()
{
}
