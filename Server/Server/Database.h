#pragma once
#include <fstream>
#include <vector>
#include <string>
class Question
{
	std::vector<std::string> answerList;
	char correctAnswer;
	std::string question;
public:
	Question();
	Question(std::vector<std::string> list,
		char correct, std::string question);
	~Question();
};
class Database
{
	std::vector<Question*> list;
public:
	Database();
	Database(std::string filename);
	~Database();
};

