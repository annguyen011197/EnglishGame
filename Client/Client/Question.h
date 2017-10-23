#pragma once
class Question
{
public:
	CStringList answerCList;

	CString question;

	Question();
	Question(CStringList *list);
	Question(char correct, CString question);
	void SetAnswerList(CStringList *answertList);
	CString getQuestion();
	void SetQuestion(CStringList *list);
	void SerializeAnswerList(CArchive &ar);
	~Question();
};

