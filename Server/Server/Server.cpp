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
//byte* Serialize(Message* msg, int &length);
Message* DeSerialize(byte* data, int len);
//CStringList* SerializQuestion(Question* quest, int &length);
byte* Serialize(int &length, CStringList* list, Message* msg);
CStringList* SerializQuestionToCStringList(Question* quest);
TCHAR* convert(string str);
bool checkQList(Question* q, vector<Question*> qList);


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
		mess->SetNewMessage(CM_PRINT, _T("Da ket noi\n Chao mung ban den voi English Game\n"));
		int len = 0;
		CStringList * tempList = new CStringList;
		tempList->AddTail(L".");
		byte* data = Serialize(len, tempList, mess);
		cout << len;
		sClient->Send(data, len);
		delete data;
		tempList->~CStringList();
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
	vector<Question*> listQ;
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
				cout << "Sending Question\n";
				Question * quest = new Question;
				do {
					quest = database->GetRandomQuestion();
				} while (checkQList(quest, listQ));
				listQ.push_back(quest);
				mess->SetNewMessage(CM_PLAY, L"");
				data = Serialize(len, SerializQuestionToCStringList(quest), mess);
				sClient->Send(data, len);
				Sleep(500);
				len = sClient->Receive(data, 256);
				mess = DeSerialize(data, len);
				if (mess->GetMess() > 100000) {
					sClient->Close();
					checkflag = 0;
					cout << "\n Closing 1 socket \n";
					pmList->DeletePlayer(ID);
					tempList->~CStringList();
					delete database;
					delete data;
					delete mess;
					delete sClient;
					delete quest;
					return 0;
					break;
				}
				else {
					char answerChar = mess->GetInfo()[0];
					if (answerChar == quest->getCorrect() || answerChar + 32 == quest->getCorrect()) {
						score++;
						wrong = 0;
						string temp = "Tra loi dung +1 d \nBan hien co:" + to_string(score) + " d";
						mess->SetNewMessage(CM_PRINT, convert(temp));
						data = Serialize(len, tempList, mess);
						sClient->Send(data, len);
						Sleep(1000);
					}
					else {
						score--;
						wrong++;
						string temp = "Tra loi sai -1 d \nBan hien co:" + to_string(score) + " d";
						mess->SetNewMessage(CM_PRINT, convert(temp));
						data = Serialize(len,tempList,mess);
						sClient->Send(data, len);
						Sleep(1000);
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

					if (listQ.size() == database->getSize()) {
						Sleep(500);
						string temp = "Ban da hoan tat so cau hoi\n Ban co" + to_string(score) + " d";
						mess->SetNewMessage(CM_PRINT, convert(temp));
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

					if (answerChar == '\0') {
						sClient->Close();
						checkflag = 0;
						cout << "\n Closing 1 socket \n";
						pmList->DeletePlayer(ID);
						tempList->~CStringList();
						delete database;
						delete data;
						delete mess;
						delete sClient;
						closesocket(hSocket);
						delete quest;
						return 0;
						break;
					}

				}
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
	closesocket(hSocket);
	return 0;
}

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

bool checkQList(Question* q, vector<Question*> qList) {
	for (auto i : qList) {
		if (i->index == q->index) {
			return true;
		}
	}
	return false;
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