#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

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

void main()
{
	int lsd,sd,choice,response,playercount = 0;
	int fd[2];

	pipe(fd);

	lsd = socket(AF_INET,SOCK_STREAM,0);						//setting up listen sd

	int * Players = (int *)calloc(nPLAYERS,sizeof(int));		//stores the values of socket descriptaars which are actually integers
	int * PlayerState = (int *)calloc(nPLAYERS,sizeof(int));   	//All players will be in lobby initially

	struct sockaddr_in clntadd;									
	
	//Set up Server socket address
	struct sockaddr_in servadd;
	servadd.sin_family = AF_INET;
	servadd.sin_addr.s_addr = inet_addr("127.0.0.1");
	servadd.sin_port = htons(9407);

	//Bind the socket address with the client
	int bind_result = bind(lsd,(struct sockaddr *)&servadd,sizeof(servadd));
	if(bind_result==0)
	printf("Bind Successful\n");
	else
	printf("Bind Failed\n");	

	listen(lsd,5);												//TODO not sure whether 5 is OK

	int clntAddLen;

	for(;;)
	{
		sd = accept(lsd,(struct sockaddr *)&clntadd,&clntAddLen);


		//Add socket descriptaaar to our list of Players
		//Set the state to INLOBBY

		PlayerState[GetNextEmptyIndex(PlayerState)] = INLOBBY;
		Players[GetNextEmptyIndex(Players)] = sd;

		playercount++;										//Keeps count of total number of players. Used later to get the current players index

		//Accept Player choice
		recv(sd,(int *)&choice,sizeof(choice),0);


		if(choice == 1)										//Player chooses play
		{
			printf("Player chose to Play\n");

			int thisPlayerIndex = playercount - 1;				//thisPlayerIndex will refer to the index of the current Player


			//Check the PlayerState array and find the first waiting player
			int CompetitorIndex = GetFirstWaitingPlayer(PlayerState);

			//If a competitor is found
			if(CompetitorIndex != -1)
			{
				printf("Player found at %d\n",CompetitorIndex );

				//Send PLAYERFOUND to the player whose index was received
				response = PLAYERFOUND;
				send(sd,(int *)&response,sizeof(response),0);

				//Set PlayerState of current Player to PLAYING
				PlayerState[thisPlayerIndex] = PLAYING;

				//Set PlayerState of waiting player to PLAYING
				PlayerState[CompetitorIndex] = PLAYING;

				//Tell the waiting Player that a dude has been found
				response = PLAYERFOUND;
				send(Players[CompetitorIndex],(int *)&response,sizeof(response),0);

				if(fork())
				{

					//TODO KRITIK Set up a match between current player and the player whose index we just received

					//the socket descriptaars of the two players can be accesed through Players[thisPlayerIndex] and Players[CompetitorIndex]

				}

			}//No Player was waiting 
			else
			{
				printf("No Player was found\n");

				//Send Player communication code NOPLAYERFOUND
				response = NOPLAYERFOUND;
				send(Players[CompetitorIndex],(int *)&response,sizeof(response),0);

				printf("PLayer was told to wait\n");
				//Set PlayerState of the current Player to waiting
				PlayerState[thisPlayerIndex] = WAITING;
				
				//ready communication from a process which wants to play
			}

		}
		else//Player chooses spectate
		{
			//TODO KRITIK implement code here for spectating
			printf("Player chose to spectate\n");
		}
	}
}

