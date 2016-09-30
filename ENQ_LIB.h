/*
	Class definitions:-
		Question
		Game
	Utility Functions:-
		bool isExists(int *, int, int);
		void getRandoms(int *, int);
*/


#include<string>
#include<fstream>
#include<list>
#include<cstdlib>
#include<ctime>
#define GAME_LENGTH 10

using namespace std;


bool isExists(int * arr, int size, int key)
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
			gen = rand() % 50;
		} while (isExists(arr, i, gen));

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

private:
	string question;
	string optionA, optionB, optionC, optionD;
	char correctAnswer;

public:
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

	void saveToFile(string filePath)
	{
		fstream qFile;
		qFile.open(filePath, ios::ate | ios::binary);
		qFile.write((char *)this, sizeof(Question));
		n_Questions++;
	}
};

class Game
{

	/*
		This sets up the game environment. Generates a sequence of random numbers and fetches the corresponding questions from a file.
		Also handles the game scores and the UI
		Author: Kritik
	*/

	list<Question> quiz;
	
	Game(string filePath)
	{
		int sequence[GAME_LENGTH];
		getRandoms(sequence, GAME_LENGTH);

		//get questions from 'filePath'
	}

	
};