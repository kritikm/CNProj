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

	send(sd,(int *)&choice,sizeof(choice),0);

	//Get the server response on whether the Player should wait or start match
	recv(sd,(int *)&ServerResponse,sizeof(ServerResponse),0);

   	//if No players are found
    while(ServerResponse == NOPLAYERFOUND && ServerResponse != PLAYERFOUND)
	{
		//wait here until the server sends message PLAYERFOUND
		recv(sd,(int *)&ServerResponse,sizeof(ServerResponse),0);	//I assume here that the recv message waits for a response. If not, use read
		//put in while in case we get NOPLAYERFOUND again
	}

	//If you have reached this point it means a player has been found

	//TODO KRITIK implement code here to handle the game

	return 0;

}
