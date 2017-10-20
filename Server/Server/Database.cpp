#include "stdafx.h"
#include "Database.h"


Database::Database()
{
}

Database::Database(std::string filename)
{
	std::fstream f;
	f.open(filename, std::ios_base::in);
	int count;
	std::string temp;
	f >> count;
	for(int i=0;i<count;i++) {
		std::string question;
		std::vector<std::string> answer;
		char correct;
		std::getline(f, temp);
		std::getline(f, question);
		for (int j = 0; j < 4; j++) {
			std::getline(f, temp);
			answer.push_back(temp);
		}
		f >> correct;
		Question *newQuestion = new Question(answer, correct, question);
		list.push_back(newQuestion);
	}
}


Database::~Database()
{
	for (int i = 0; i < list.size(); i++) {
		delete list[i];
		list.pop_back();
	}
}

Question::Question()
{
	correctAnswer = NULL;
	question = "";
}

Question::Question(std::vector<std::string> list, char correct, std::string Question)
{
	answerList = list;
	correctAnswer = correct;
	question = Question;
}

Question::~Question()
{
}
