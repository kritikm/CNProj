#pragma once
#include<string>
#include<fstream>
#include<list>

using namespace std;

class Option
{
public:
	string option;
	bool isCorrect;

	Option(string toOption, bool toIsCorrect)
	{
		option = toOption;
		isCorrect = toIsCorrect;
	}

	Option()
	{
		option = "";
		isCorrect = false;
	}
};

class Question
{
public:
	int qID;
	string question;
	Option optionA, optionB, optionC, optionD;

	Question() { question = ""; }

	Question(string toQuestion, string toA, string toB, string toC, string toD, char isCorrect)
	{
		Question c;
		question = toQuestion;
		optionA.option = toA;
		optionB.option = toB;
		optionC.option = toC;
		optionD.option = toD;

		switch (isCorrect)
		{
		case 'a':
			optionA.isCorrect = true;
			break;

		case 'b':
			optionB.isCorrect = true;
			break;

		case 'c':
			optionC.isCorrect = true;
			break;

		case 'd':
			optionD.isCorrect = true;

		default:
			break;
		}
	}

	void saveToFile(string filePath)
	{
		fstream qFile;
		qFile.open(filePath, ios::ate | ios::binary);
		qFile.write((char *)this, sizeof(Question));
	}
};

class Game
{
	list<Question> quiz;
	
};