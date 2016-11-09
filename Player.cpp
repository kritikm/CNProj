#include<iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<string>

using namespace std;

#define PLAYERFOUND 1
#define NOPLAYERFOUND -1

void setUpPlayArea(int sequence, string question, string optionA, string optionB, string optionC, string optionD)
{
    cout<<"**********\nQuestion "<<sequence<<"\n\n";

    cout<<question<<"\n\nA:"<<optionA<<"\nB:"<<optionB<<"\nC:"<<optionC<<"\nD:"<<optionD<<endl;
    cout<<"Your answer here: ";

}

int main()
{
	int sd,choice,ServerResponse;

	struct sockaddr_in servadd;

	servadd.sin_family = AF_INET;
	servadd.sin_addr.s_addr = inet_addr("10.0.2.15");
	servadd.sin_port = htons(9407);

	sd = socket(AF_INET,SOCK_STREAM,0);

	connect(sd,(struct sockaddr *)&servadd,sizeof(servadd));

	cout<<"Welcome to ENQ\n";
	cout<<"What would you like to do?\n";
	cout<<"1 : Play  2 : Spectate\n";
	cin>>choice;

	write(sd,&choice,sizeof(&choice));

	//Get the server response on whether the Player should wait or start match
	read(sd,&ServerResponse,sizeof(ServerResponse));

   	//if No players are found
    while(ServerResponse == NOPLAYERFOUND && ServerResponse != PLAYERFOUND)
	{
		//wait here until the server sends message PLAYERFOUND
		read(sd,&ServerResponse,sizeof(ServerResponse));	//I assume here that the recv message waits for a response. If not, use read
		//put in while in case we get NOPLAYERFOUND again
	}

	//If you have reached this point it means a player has been found

    cout<<"Game started!\n";
	char * buf;
    char correctAnswer;
    char playerAnswer;
    int length;
    int score = 0;
    int gameLength;

    read(sd, &gameLength, sizeof(gameLength));
    cout<<gameLength<<endl;

    for(int i = 0; i < gameLength; i++)
    {
        playerAnswer = 'y';
        read(sd, &length, sizeof(length));
        buf = new char[length];
        read(sd, buf, length + 1);
        read(sd, &correctAnswer, sizeof(length));


        string receivedQuestion(buf);

        string question = receivedQuestion.substr(0, receivedQuestion.find('?') + 1);
        receivedQuestion = receivedQuestion.substr(receivedQuestion.find('?') + 1);
        string optionA = receivedQuestion.substr(0, receivedQuestion.find('?'));
        receivedQuestion = receivedQuestion.substr(receivedQuestion.find('?') + 1);
        string optionB = receivedQuestion.substr(0, receivedQuestion.find('?'));
        receivedQuestion = receivedQuestion.substr(receivedQuestion.find('?') + 1);
        string optionC = receivedQuestion.substr(0, receivedQuestion.find('?'));
        string optionD = receivedQuestion.substr(receivedQuestion.find('?') + 1);

        setUpPlayArea(i + 1, question, optionA, optionB, optionC, optionD);

        cin>>playerAnswer;
        int state = send(sd, (char *)&playerAnswer, sizeof(length), 0);
        cout<<state<<endl;
        if(playerAnswer == correctAnswer)
        {
            score++;
            cout<<"\nCorrect answer!\n";
        }
        else
        {
            cout<<"\nIncorrect Answer!\n";
        }
        cout<<"Your score is "<<score<<endl;
    }


//	int ques = read(sd, buf, sizeof(buf));
//	cout<<"Received quest "<<ques<<endl;
//	read(sd, &correctAnswer, sizeof(correctAnswer));
//
//    cout<<"Server sent "<<buf<<" and the correct answer is "<<correctAnswer;
	//TODO KRITIK implement code here to handle the game

	return 0;

}
