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
		CStringList *answer = new CStringList;
		char correct;
		std::getline(f, temp);
		std::getline(f, question);
		for (int j = 0; j < 4; j++) {
			std::getline(f, temp);
			answer->AddTail(convert(temp));
		}
		f >> correct;
		Question *newQuestion = new Question( correct, convert(question));
		newQuestion->SetAnswerList(answer);
		list.push_back(newQuestion);
		delete answer;
	}
}

Question * Database::GetRandomQuestion()
{
	srand(time(NULL));
	int ran = rand() % list.size();
	return list[ran];
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

Question::Question(CStringList list, char correct, CString Question)
{

	for (POSITION i = list.GetHeadPosition(); i !=NULL;) {
		answerCList.AddTail(list.GetNext(i));
	}
	correctAnswer = correct;
	question = Question;
}

Question::Question(char correct, CString _question)
{
	correctAnswer = correct;
	question = _question;
}

void Question::SetAnswerList(CStringList *answertList)
{
	for (POSITION i = answertList->GetHeadPosition(); i != NULL;) {
		answerCList.AddTail(answertList->GetNext(i));
	}
}

CString Question::getQuestion()
{
	return question;
}

void Question::SerializeAnswerList(CArchive &ar)
{
	this->answerCList.Serialize(ar);
}

char Question::getCorrect()
{
	return correctAnswer;
}


Question::~Question()
{
}
