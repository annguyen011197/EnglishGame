#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
class Question
{
	char correctAnswer;
	CString question;
public:
	Question();
	Question(CStringList list,
		char correct, CString question);
	Question(char correct, CString question);
	void SetAnswerList(CStringList *answertList);
	CString getQuestion();
	void SerializeAnswerList(CArchive &ar);
	char getCorrect();
	CStringList answerCList;
	~Question();
};
class Database
{
	std::vector<Question*> list;
public:
	Database();
	Database(std::string filename);
	Question* GetRandomQuestion();
	TCHAR* convert(std::string str)
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
	~Database();
};


