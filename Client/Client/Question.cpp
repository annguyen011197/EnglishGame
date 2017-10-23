#include "stdafx.h"
#include "Question.h"


Question::Question()
{
	question = "";
}

Question::Question(CStringList *list)
{
	question = list->GetHead();
	POSITION i = list->GetHeadPosition();
	list->GetNext(i);
	while (i != NULL) {
		answerCList.AddTail(list->GetNext(i));
	}
}

Question::Question(char correct, CString _question)
{
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

void Question::SetQuestion(CStringList * list)
{
	question = list->GetHead();
	POSITION i = list->GetHeadPosition();
	list->GetNext(i);
	while (i != NULL) {
		answerCList.AddTail(list->GetNext(i));
	}
}

void Question::SerializeAnswerList(CArchive &ar)
{
	this->answerCList.Serialize(ar);
}



Question::~Question()
{
}

