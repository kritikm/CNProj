/*
	Class definitions:-
		Question
		Game

	Utility Functions:-
		bool Exists(int *, int, int);
		void getRandoms(int *, int);

	Pre processor constants:-
		GAME_LENGTH - number of questions in a game
*/


#include<string>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>

#define GAME_LENGTH 2
#define PORT 9408

using namespace std;


bool Exists(int * arr, int size, int key)
{
	for (int i = 0; i < size; i++)
		if (arr[i] == key)
			return true;
	return false;
}

void getRandoms(int * arr, int size)
{
	srand(time(0));

	for (int i = 0; i < size; i++)
	{
		int gen;

		do {
			gen = rand() % size + 1;
		} while (Exists(arr, i, gen));

		arr[i] = gen;
	}
}


class Question
{
	/*
		Each question will be an object of this class. A question has a string which holds the question itself and 4 instances of Option type
		Creates a question and saves it to a file
		Author: Kritik
	*/

public:
	string question;
	string optionA, optionB, optionC, optionD;
	char correctAnswer;

	static int n_Questions;

	Question(string toQuestion = "", string toA = "", string toB = "", string toC = "", string toD = "", char isCorrect = 'a')
	{

		question = toQuestion;
		optionA = toA;
		optionB = toB;
		optionC = toC;
		optionD = toD;
		correctAnswer = isCorrect;

	}
};

class Game
{

	/*
		This sets up the game environment. Generates a sequence of random numbers and fetches the corresponding questions from a file.
		Also handles the game scores and the UI
		Author: Kritik
	*/
public:
	vector<Question> quiz;
	int playerOne, playerTwo;
	int playerOneScore, playerTwoScore;

	Game(int toPlayerOne, int toPlayerTwo)
	{
		int sequence[GAME_LENGTH];
		getRandoms(sequence, GAME_LENGTH);
		playerOneScore = 0;
		playerTwoScore = 0;
		playerOne = toPlayerOne;
		playerTwo = toPlayerTwo;

		for(int i = 0; i < GAME_LENGTH; i++)
		{
			quiz.push_back(getQuestion(sequence[i]));
        }

		//get questions from 'filePath'
	}

	Question getQuestion(int fileNumber)
	{
		string filePath = to_string(fileNumber).append(".txt");
		Question quest;

		ifstream file(filePath.c_str());

		getline(file, quest.question);
		getline(file, quest.optionA);
		getline(file, quest.optionB);
		getline(file, quest.optionC);
		getline(file, quest.optionD);

		string getAnswer;
		getline(file, getAnswer);

		quest.correctAnswer = getAnswer.at(0);

		return quest;
	}
};

