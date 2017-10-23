#include "stdafx.h"
#include "Message.h"


Message::Message()
{
}

Message::Message(int nMess, CString sInfo)
{
	m_Message = nMess;
	info = sInfo;
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
