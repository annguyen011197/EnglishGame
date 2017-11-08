#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <tchar.h>
class Question
{
	char correctAnswer;
	CString question;
public:
	int index;
	Question();
	Question(CStringList list,
		char correct, CString question, int index);
	Question(char correct, CString question, int index);
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
	TCHAR* cvt;
public:
	Database();
	Database(std::string filename);
	Question* GetRandomQuestion();
	void convert(TCHAR* res,std::string str);
	int getSize();
	~Database();
};


