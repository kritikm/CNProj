#include<iostream>
#include <fcntl.h>
#include <stdlib.h>
#include<sys/mman.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<vector>
#include<thread>
#include<future>
#include<string.h>
#include"ENQ_LIB.h"

using namespace std;

#define nPLAYERS 10

#define EMPTY      0
#define PLAYING    1
#define WAITING    2
#define SPECTATING 3
#define INLOBBY    4

//COMMUNICATION CODES
#define PLAYERFOUND 1
#define NOPLAYERFOUND -1

//Returns index of the next empty slot in the array
int GetNextEmptyIndex(int * arr)
{
	int i=0;
	while(arr[i]!=0 && i<nPLAYERS)
		i++;
																//TODO Handle condition for out of bounds
	return i;
}

//Returns index of the next waiting player, -1 if no waiting player found
int GetFirstWaitingPlayer(int * playerState)
{
	int i;

	for(i=0;i<nPLAYERS;i++)
	{
		if(playerState[i] == WAITING)
		{
			return i;
		}
	}

	return -1;
}

//Sends a question to the player and gets his / her answer
int sendQuestion(Question toSend, int playerIndex)
{
    string question = toSend.question + toSend.optionA + toSend.optionC + toSend.optionD + toSend.correctAnswer;
    write(playerIndex, question.c_str(), question.length());
    char submittedAnswer;
    read(playerIndex, &submittedAnswer, sizeof(submittedAnswer));

    return submittedAnswer;

}

int main()
{
	int lsd,sd,choice,response,playercount = 0;
	int fd[2];

	pipe(fd);

	lsd = socket(AF_INET,SOCK_STREAM,0);						//setting up listen sd

	int *Players = (int *)mmap(0, nPLAYERS * sizeof(int),
                    PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
                    -1, 0);                   		//stores the values of socket descriptaars which are actually integers
    int *PlayerState = (int *)mmap(0, nPLAYERS * sizeof(int),
                        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
                        -1, 0);                  	//All players will be in lobby initially


	struct sockaddr_in clntadd;

	//Set up Server socket address
	struct sockaddr_in servadd;
	servadd.sin_family = AF_INET;
	servadd.sin_addr.s_addr = inet_addr("10.0.2.15");
	servadd.sin_port = htons(9407);

	//Bind the socket address with the client
	int bind_result = bind(lsd,(struct sockaddr *)&servadd,sizeof(servadd));
	if(bind_result==0)
	cout<<"Bind Successful\n";
	else
	cout<<"Bind Failed\n";

	listen(lsd,5);												//TODO not sure whether 5 is OK

	socklen_t clntAddLen;

	for(;;)
	{
		sd = accept(lsd,(struct sockaddr *)&clntadd,&clntAddLen);
        cout<<sd<<endl;

		//Add socket descriptaaar to our list of Players
		//Set the state to INLOBBY

		PlayerState[GetNextEmptyIndex(PlayerState)] = INLOBBY;
		Players[GetNextEmptyIndex(Players)] = sd;

		playercount++;										//Keeps count of total number of players. Used later to get the current players index

		//Accept Player choice
		recv(sd,(int *)&choice,sizeof(choice),0);

		if(choice == 1)										//Player chooses play
		{
			cout<<"Player chose to Play\n";

            int * thisPlayerIndex = (int *)mmap(0, sizeof(int), PROT_READ | PROT_WRITE,
                                                MAP_ANONYMOUS | MAP_SHARED,
                                                -1, 0);
            *thisPlayerIndex = playercount - 1;             //thisPlayerIndex will refer to the index of the current Player

			//Check the PlayerState array and find the first waiting player

			int * competitorIndex = (int *)mmap(0, sizeof(int), PROT_WRITE | PROT_READ,
                                                MAP_ANONYMOUS | MAP_SHARED,
                                                -1, 0);
			*competitorIndex = GetFirstWaitingPlayer(PlayerState);

			//If a competitor is found
			if(*competitorIndex != -1)
			{
				cout<<"Player found at "<<*competitorIndex<<endl;

				//Send PLAYERFOUND to the player whose index was received
				response = PLAYERFOUND;
				send(sd,(int *)&response,sizeof(response),0);

				//Set PlayerState of current Player to PLAYING
				PlayerState[*thisPlayerIndex] = PLAYING;

				//Set PlayerState of waiting player to PLAYING
				PlayerState[*competitorIndex] = PLAYING;
				//Tell the waiting Player that a dude has been found
				response = PLAYERFOUND;
				send(Players[*competitorIndex],(int *)&response,sizeof(response),0);

				if(fork())
				{
                    cout<<"Player States\n";
                    for(int i = 0; i <= *thisPlayerIndex; i++)
                    {
                        cout<<PlayerState[i]<<" ";
                    }
                    cout<<endl;
                    cout<<"Players\n";
                    for(int i = 0; i <= *thisPlayerIndex; i++)
                    {
                        cout<<Players[i]<<" ";
                    }
                    cout<<endl;

                    vector<Question> questions;
                    questions.push_back(Question("Who am I?", "God", "Kritik", "Batman", "John Cena", 'b'));
                    questions.push_back(Question("What are you?", "Man", "Funny", "Punny", "Never gonna give you up", 'd'));


                    Question toSend = questions.at(0);
                    string toSendString = toSend.question + toSend.optionA + toSend.optionB + toSend.optionC + toSend.optionD;
                    int length = toSendString.length();
                    char * buf = strdup(toSendString.c_str());
                    char correctAnswer = toSend.correctAnswer;

                    cout<<"Sending length "<<length<<endl;
                    send(Players[*thisPlayerIndex], (int *)&length, sizeof(length), 0);
                    send(Players[*competitorIndex], (int *)&length, sizeof(length), 0);

                    cout<<"Sending Question "<<buf<<endl;
                    send(Players[*thisPlayerIndex], buf, length, 0);
                    send(Players[*competitorIndex], buf, length, 0);

                    cout<<"Sending answer "<<correctAnswer<<endl;
                    send(Players[*thisPlayerIndex], &correctAnswer, sizeof(correctAnswer), 0);
                    send(Players[*competitorIndex], &correctAnswer, sizeof(correctAnswer), 0);
//                    write(Players[*competitorIndex], &correctAnswer, sizeof(&correctAnswer));
//                    write(Players[*thisPlayerIndex], buf, sizeof(buf));
//                    write(Players[*thisPlayerIndex], &correctAnswer, sizeof(&correctAnswer));


//                    auto playerOneAnswer = async(sendQuestion, question, Players[competitorIndex]);
//                    auto playerTwoAnswer = async(sendQuestion, question, Players[thisPlayerIndex]);

//                    cout<<"Player one answered "<<playerOneAnswer<<" and player two answered "<<playerTwoAnswer<<endl;



					//TODO KRITIK Set up a match between current player and the player whose index we just received

					//the socket descriptaars of the two players can be accesed through Players[thisPlayerIndex] and Players[competitorIndex]

					close(Players[*thisPlayerIndex]);
					close(Players[*competitorIndex]);
                    PlayerState[*thisPlayerIndex] = 0;
                    Players[*thisPlayerIndex] = 0;
                    PlayerState[*competitorIndex] = 0;
                    Players[*competitorIndex] = 0;
				}

			}//No Player was waiting
			else
			{
				cout<<"No Player was found\n";

				//Send Player communication code NOPLAYERFOUND
				response = NOPLAYERFOUND;
				send(Players[*competitorIndex],(int *)&response,sizeof(response),0);

				cout<<"PLayer was told to wait\n";
				//Set PlayerState of the current Player to waiting
				PlayerState[*thisPlayerIndex] = WAITING;

				//ready communication from a process which wants to play
			}

		}
		else//Player chooses spectate
		{
			//TODO KRITIK implement code here for spectating
			cout<<"Player chose to spectate\n";
		}
	}

	return 0;
}

