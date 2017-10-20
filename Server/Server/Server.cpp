// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include "PlayerManager.h"
#include "Database.h"
#include <iostream>

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
	Database * data = new Database("Database.txt");
	pmList = new PlayerManager;
	sServer->Listen();
	while (sServer->Accept(*sClient)) {
		cout << "Mot ket noi moi \n";
		sClient->Send("Da ket noi",32);
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
}

UINT ClientFunc(LPVOID pParam)
{
	SOCKET hSocket;
	hSocket = (SOCKET)pParam;
	CSocket * sClient = new CSocket;
	sClient->Attach(hSocket);
	char* rStr = new char[100];
	char* sStr = new char[100];
	int len = 0;
	while (1) {
		len=sClient->Receive(rStr, 100);
		switch (GetMessageClient(rStr)) {
		case CM_REG:
		{
			string ID;
			for (int i = 3; rStr[i] != '\0'; i++) {
				ID.push_back(rStr[i]);
			}
			if (pmList->CheckID(ID)) {
				pmList->AddNewPlayer(ID);
				cout << "\n Da dang ky 1 Nguoi choi\n";
			}
			else {
				sClient->Send("Error1", 20);
			}
			break;
		}
		}
	}
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
