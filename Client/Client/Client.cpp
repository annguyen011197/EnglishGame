// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
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
bool IDMessage(string str, char res[]);

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
	//sClient.Connect(convert(IP), nPort);
	char rStr[100];
	int len = 0;
	TCHAR * temp = convert(IP);
	if (sClient.Connect(convert(IP), nPort)) {
		while (1) {
			len = sClient.Receive(rStr, 100);
			rStr[len] = 0;
			cout << rStr <<endl;
			cout << "Nhap ID: ";
			char res[100];
			while (1) {
				while (1) {
					getline(cin, IP);
					if (IDMessage(IP, res)) {
						break;
					}
				}
				sClient.Send(res, 100);
				len = sClient.Receive(rStr, 100);
				rStr[len] = 0;
				if (strcmp("Error1", rStr)==0) {
					break;
				}
			}
			sClient.Send(res, strlen(res));
		}
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

bool IDMessage(string str, char res[])
{
	if (str.size() > 8) {
		cout << "ID duoi 8 ky tu\n";
		return false;
	}
	str = "REG" + str;
	for (int i = 0; i < str.size(); ++i) {
		res[i] = str[i];
	}
	res[str.size()] = 0;
	return true;
}
