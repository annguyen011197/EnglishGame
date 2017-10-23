// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include "PlayerManager.h"
#include "Database.h"
#include "Message.h"
#include <windows.h>
#include <iostream>
#include <future>       
#include <chrono> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;
PlayerManager* pmList;

using namespace std;

void MainFunc();
UINT __cdecl ClientFunc(LPVOID pParam);
int GetMessageClient(char* str);
//byte* Serialize(Message* msg, int &length);
Message* DeSerialize(byte* data, int len);
//CStringList* SerializQuestion(Question* quest, int &length);
byte* Serialize(int &length, CStringList* list, Message* msg);
CStringList* SerializQuestionToCStringList(Question* quest);
TCHAR* convert(string str);

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			MainFunc();

        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}


void MainFunc() {
	int nPort;
	CSocket * sServer = new CSocket;
	if (!AfxSocketInit()) {
		cout << "Khoi tao Socket loi\n";
		return;
	}
	cout << "Nhap port: ";
	cin >> nPort;
	if (!sServer->Create(nPort)) {
		cout << "Khoi tao Socket loi\n";
		return;
	}
	CSocket * sClient = new CSocket;
	Message * mess = new Message;
	pmList = new PlayerManager;
	sServer->Listen();
	while (sServer->Accept(*sClient)) {
		cout << "Mot ket noi moi \n";
		mess->SetNewMessage(CM_PRINT, _T("Da ket noi"));
		int len = 0;
		CStringList * tempList = new CStringList;
		tempList->AddTail(L".");
		byte* data = Serialize(len, tempList, mess);
		sClient->Send(data, len);
		delete data;
		delete tempList;
		SOCKET  hSocket = sClient->Detach();
		AfxBeginThread(
			ClientFunc,
			(LPVOID)hSocket
		);
	}
	sServer->Close();
	sClient->Close();
	delete sServer;
	delete sClient;
	delete mess;
}

UINT ClientFunc(LPVOID pParam)
{
	int len = 0;
	SOCKET hSocket;
	hSocket = (SOCKET)pParam;
	CSocket * sClient = new CSocket;
	sClient->Attach(hSocket);
	Message * mess = new Message;
	Database * database = new Database("Database.txt");
	byte* data = new byte;
	CStringList * tempList = new CStringList;
	tempList->AddTail(L".");
	int ID;
	mess->SetNewMessage(CM_REG, _T(""));
	data = Serialize(len, tempList, mess);
	sClient->Send(data, len);
	bool checkflag = 1;
	while (checkflag) {
		len=sClient->Receive(data, 256);
		mess = DeSerialize(data, len);
		switch (mess->GetMess()) {
		case CM_REG: {
			if (mess->GetInfo() == L"") {
				sClient->Close();
				checkflag = 0;
				break;
			}
				if (pmList->CheckID(mess->GetInfo())) {
					pmList->AddNewPlayer(mess->GetInfo());
					ID = pmList->getSize()-1;
					cout << "\n Da dang ky 1 Nguoi choi\n";
					mess->SetNewMessage(CM_PRINT, _T("Dang ky thanh cong"));
					data = Serialize(len, tempList, mess);
					sClient->Send(data, len);
					break;
				}
				else {
					mess->SetNewMessage(ERRORDUP, _T("ID da ton tai"));
					data = Serialize(len, tempList, mess);
					sClient->Send(data, len);
				}
			break;
		}
		case CM_PLAY:
		{
			mess->SetNewMessage(CM_PRINT, _T("Bat dau tro choi"));
			data = Serialize(len, tempList, mess);
			sClient->Send(data, len);
			cout << "Sending Start";
			int score = 3;
			int wrong = 0;
			while (1) {
				Sleep(1000);
				mess->SetNewMessage(CM_PLAY, _T("."));
				data = Serialize(len, tempList, mess);
				sClient->Send(data, len);
				cout << "Sending Question";
				Sleep(1000);
				Question * quest = new Question;
				quest = database->GetRandomQuestion();
				mess->SetNewMessage(CM_QUEST, L"");
				data = Serialize(len, SerializQuestionToCStringList(quest), mess);
				//data = SerializQuestionToCStringList(quest);
				sClient->Send(data, len);
				Sleep(500);
				len = sClient->Receive(data, 256);
				mess = DeSerialize(data, len);
				if (mess->GetMess() > 100000) {
					sClient->Close();
					checkflag = 0;
					cout << "\n Closing 1 socket \n";
					pmList->DeletePlayer(ID);
					delete database;
					delete data;
					delete mess;
					delete sClient;
					return 0;
					break;
				}
				else {
					char answerChar = mess->GetInfo()[0];
					if (answerChar == quest->getCorrect() || answerChar + 32 == quest->getCorrect()) {
						score++;
						string temp = "Tra loi dung +1 d \nBan hien co:" + to_string(score) + " d";
						mess->SetNewMessage(CM_PRINT, convert(temp));
						data = Serialize(len, tempList, mess);
						sClient->Send(data, len);
					}
					else {
						score--;
						wrong++;
						string temp = "Tra loi sai -1 d \nBan hien co:" + to_string(score) + " d";
						mess->SetNewMessage(CM_PRINT, convert(temp));
						data = Serialize(len,tempList,mess);
						sClient->Send(data, len);
					}

					if (wrong == 3 || score <= 0) {
						Sleep(500);
						mess->SetNewMessage(CM_PRINT, _T("Ban da thua cuoc"));
						data = Serialize(len, tempList, mess);
						Sleep(500);
						sClient->Send(data, len);
						Sleep(500);
						mess->SetNewMessage(CM_EXIT, _T(""));
						data = Serialize(len, tempList, mess);
						sClient->Send(data, len);
						Sleep(1000);
						sClient->Close();
						pmList->DeletePlayer(ID);
						break;
					}
				}

				//future<bool> fut = async([hSocket,&score](SOCKET socket,int score,Question* quest,byte *data) {
				//	CSocket sClient;
				//	sClient.Attach(socket);
				//	int len = 0;
				//	Message * mess = new Message;
				//	}
				//	delete mess;
				//	return true;
				//}, hSocket,score,quest,data);
				//chrono::milliseconds span(1000);
				//while (fut.wait_for(span)==future_status::timeout) {
				//	cout << "Doi cau trl";
				//}
			}
			break;
		}
		case CM_EXIT:
		{
			checkflag = 0;
			break;
		}
		}
	}
	delete database;
	delete data;
	delete mess;
	delete sClient;
	return 0;
}

int GetMessageClient(char * str)
{
	string res;
	for (int i = 0; i < 3; ++i) {
		res.push_back(str[i]);
	}
	if (res == "REG") {
		return CM_REG;
	}
	return 0;
}

/*byte* Serialize(Message* msg, int &length) {
	CMemFile memfile;
	CArchive archive(&memfile, CArchive::store);
	archive << msg->GetMess();
	archive << msg->GetInfo();
	archive.Close();
	length = (INT)memfile.GetLength();
	BYTE* data = memfile.Detach();
	memfile.Abort();
	return data;
}*/

Message* DeSerialize(byte* data, int len) {
	CStringList *list = new CStringList;
	CMemFile* memfile = new CMemFile;
	memfile->Attach(data, len);
	CArchive *archive= new CArchive(memfile, CArchive::load);
	int nMess;
	CString cInfo;
	*archive >> nMess;
	*archive >> cInfo;
	list->Serialize(*archive);
	archive->Close();
	delete memfile;
	delete archive;
	Message* mess = new Message(nMess, cInfo);
	return mess;
}

byte* Serialize(int &length, CStringList* list, Message* msg) {
	CMemFile memfile;
	CArchive archive(&memfile, CArchive::store);
	archive << msg->GetMess();
	archive << msg->GetInfo();
	list->Serialize(archive);
	archive.Close();
	length = (INT)memfile.GetLength();
	BYTE* data = memfile.Detach();
	memfile.Abort();
	return data;
}

CStringList* SerializQuestionToCStringList(Question* quest) {
	CStringList* serialList = new CStringList;
	serialList->AddTail(quest->getQuestion());
	for (POSITION i = quest->answerCList.GetHeadPosition(); i != NULL;) {
		serialList->AddTail(quest->answerCList.GetNext(i));
	}
	return serialList;
}


TCHAR* convert(string str)
{
	TCHAR * res = new TCHAR[100];
	int count = 0;
	for (auto i : str) {
		res[count] = i;
		count++;
	}
	res[count] = 0;
	return res;
}