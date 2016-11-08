#include<iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define PLAYERFOUND 1
#define NOPLAYERFOUND -1

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
    int length;
    read(sd, &length, sizeof(length));
    read(sd, buf, length);
    read(sd, &correctAnswer, sizeof(correctAnswer));

    cout<<"The correct answer is\n"<<correctAnswer<<endl;

//	int ques = read(sd, buf, sizeof(buf));
//	cout<<"Received quest "<<ques<<endl;
//	read(sd, &correctAnswer, sizeof(correctAnswer));
//
//    cout<<"Server sent "<<buf<<" and the correct answer is "<<correctAnswer;
	//TODO KRITIK implement code here to handle the game

	return 0;

}
