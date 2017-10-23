// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include "Message.h"
#include "Question.h"
#include <conio.h>
#include <string>
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void MainFunc();
TCHAR* convert(string str);
bool IDMessage(string str);
Message* DeSerialize(byte* data, int len, CStringList *quest);
byte* Serialize(int &length, CStringList* list, Message* msg);
char WaitForAnswer(int time);

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
			system("pause");
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

void MainFunc()
{
	int nPort;
	string IP;
	CSocket  sClient;
	if (!AfxSocketInit()) {
		cout << "Khoi tao Socket loi\n";
		return;
	}
	cout << "Nhap port: ";
	cin >> nPort;
	while (getchar() != '\n');
	cout << "Nhap Address: ";
	getline(cin,IP);
	sClient.Create();
	int len = 0;
	int count = 1;
	TCHAR * temp = convert(IP);
	CStringList * list = new CStringList;
	CStringList * templist = new CStringList;
	if (sClient.Connect(convert(IP), nPort)) {
		Message * mess = new Message;
		BYTE* data = new BYTE[1000];
		int check = 1;
		while (check) {
			int len = sClient.Receive(data, 100);
			mess = DeSerialize(data, len, templist);
			switch (mess->GetMess()) {
			case CM_PRINT:
			{
				wcout << mess->GetInfo().GetString() << endl;
				break;
			}
			case CM_REG: {
				bool flag = 1;
				while (flag) {
					do {
						cout << "Nhap ID: ";
						getline(cin, IP);
					} while (!IDMessage(IP));

					mess->SetNewMessage(CM_REG, convert(IP));
					data = Serialize(len, templist, mess);
					sClient.Send(data, len);
					len = sClient.Receive(data, 256);
					mess = DeSerialize(data, len,templist);
					if (mess->GetMess() == ERRORDUP) {
						wcout << mess->GetInfo().GetString() << endl;
					}
					else {
						wcout << mess->GetInfo().GetString() << endl;
						mess->SetNewMessage(CM_PLAY, convert(IP));
						data = Serialize(len, templist, mess);
						sClient.Send(data, len);
						flag = 0;
					}
				}
				break;
			}
			case CM_PLAY:
			{
				int len = sClient.Receive(data, 1000);
				Question * quest = new Question;
				mess = DeSerialize(data, len, list);
				quest->SetQuestion(list);
				wcout << quest->getQuestion().GetString() << endl;
				for (POSITION i = quest->answerCList.GetHeadPosition();
					i != NULL;) {
					wcout << quest->answerCList.GetNext(i).GetString() << endl;
				}
				cout << "Tra loi: ";
				char ans;
				do {
					ans = WaitForAnswer(60);
				} while (ans == '\n' || ans == EOF);
				cout << ans << endl;
				string a;
				a.push_back(ans);
				mess->SetNewMessage(CM_PLAY, convert(a));
				data = Serialize(len, templist, mess);
				sClient.Send(data, len);
				Sleep(1000);
				delete quest;
				break;
			}
			case CM_EXIT:
			{
				sClient.Close();
				check = 0;
				break;
			}
		}
		}
		delete mess;
	}
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

bool IDMessage(string str)
{
	if (str.size() > 8) {
		cout << "ID duoi 8 ky tu\n";
		return false;
	}
	return true;
}

Message* DeSerialize(byte* data, int len,CStringList *quest) {
	CMemFile* memfile = new CMemFile;
	memfile->Attach(data, len);
	CArchive *archive = new CArchive(memfile, CArchive::load);
	int nMess;
	CString cInfo;
	*archive >> nMess;
	*archive >> cInfo;
	quest->RemoveAll();
	quest->Serialize(*archive);
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

char WaitForAnswer(int time) {
	int direction = 1; char control;
	char  c = 0;
	bool check = 1;
	while (!_kbhit())
	{
		printf(".");
		time--;
		Sleep(1000);
		if (time < 0) {
			check = 0;
			break;
		}
	}
	if (check) {
		c = _getch();
		return c;
	}
	else {
		return '.';
	}
}
