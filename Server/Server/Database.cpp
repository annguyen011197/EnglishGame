#include "stdafx.h"
#include "Database.h"


Database::Database()
{
}

Database::Database(std::string filename)
{
	int index = 0;
	cvt = new TCHAR[100];
	std::fstream f;
	f.open(filename, std::ios_base::in);
	if (f.is_open()) {
		int count;
		std::string temp;
		std::string question;
		char correct;
		std::getline(f, temp);
		count = std::stoi(temp, NULL, 10);
		CStringList *answer = new CStringList;
		for (int i = 0; i < count; i++) {
			answer->RemoveAll();
			std::getline(f, question);
			for (int j = 0; j < 4; j++) {
				std::getline(f, temp);
				convert(cvt, temp);
				answer->AddTail(cvt);
			}
			std::getline(f, temp);
			correct = temp[0];
			convert(cvt, question);
			Question *newQuestion = new Question(correct, cvt, index);
			newQuestion->SetAnswerList(answer);
			list.push_back(newQuestion);
			index++;
		}
		answer->~CStringList();
	}
	f.close();
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

Question::Question(CStringList list,
	char correct, CString Question, int _index)
{

	for (POSITION i = list.GetHeadPosition(); i !=NULL;) {
		answerCList.AddTail(list.GetNext(i));
	}
	correctAnswer = correct;
	question = Question;
	index = _index;
}

Question::Question(char correct, CString _question, int _index)
{
	correctAnswer = correct;
	question = _question;
	index = _index;
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

void Database::convert(TCHAR* res, std::string str)
{
	int count = 0;
	for (auto i : str) {
		res[count] = i;
		count++;
	}
	res[count] = 0;
}

int Database::getSize()
{
	return list.size();
}
