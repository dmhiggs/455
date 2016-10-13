/*
	socket()
	bind()
	listen()
	accept()
	read()
	write()
	close()
*/

#include "project1.h"


//main for server to show how to go through steps
int main(int argc, char *argv[])
{
	//variables for server socket, client socket, portnumber
	//buffer
	//socket address, server address, client address, size of address?
	//others????

	int ServerSock; //server socket descriptor
	int ClientSock; //client socket descriptor
	struct sockaddr_in ServerAddress; //server address, local address
	struct sockaddr_in ClientAddress; //client address
	unsigned short ServerPort; //server port
	unsigned int ClientLen; //length of client address data structure
	
	char buffer[bufferSize]; //recv string into this
	char *message, *bytemsg; //send this string
	unsigned int messageLen; //length of message

	int bytesRcvd;
	int totalBytesRcvd;
	int i, j, cmd, recvs;

	//need 2 arguments
	if (argc != 2)
	{
		exit(1);
	}

	ServerPort = atoi(argv[1]); //second arg: local port


	//call socket(), make socket
	if ((ServerSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
	{
		perror("socket failed");
		exit(1);
	}

	memset(&ServerAddress,0,sizeof(ServerAddress)); 
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); //any incoming interface
	ServerAddress.sin_port = htons(ServerPort); //local port
	

	//bind socket to server
	if (bind(ServerSock, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))<0)
	{
		perror("bind failed");
		exit(1);
	}
	

	//listen for clients....(go in sleep mode and wait for incoming connection)
	if (listen(ServerSock, MAXPENDING < 0)
	{
		perror("listen failed");
		exit(1);
	}

	for(;;) //run forever
	{
		//set size of the in-out parameter
		ClientLen = sizeof(ClientAddress);
	

	//accept connection from client
		if ((ClientSock = accept(ServerSock, (struct sockaddr *) &ClientAddress, &ClientLen))<0)
		{
			perror("accept failed");
			exit(1);
		}

		//client is connected
		
		//receive message from client
		if ((bytesRcvd = recv(ClientSock, buffer, bufferSize, 0))<0)
		{
			perror("recv failed");
			exit(1);
		}

		//send and receive until transmission end
		while (bytesRcvd > 0)
		{

			//client message -- first byte = cmd number
			cmd = atoi(buffer[0]);

			//make message null for next command from client
			memset(message, '\0', bufferSize-1);

			//all sends use command name
			strcat(message, commands[cmd-1].name);
			messageLen = strlen(message);
			bytemsg = (char*)malloc(bufferSize);
			bytemsg = &buffer[1]; //idk if this will work...

			//handle commands from client
			switch (cmd)
			{	
			//1-4 handled the same by server
			//reply with commandname: received value
				//nullTerminated
				case 1:
				//givenLength
				case 2:
					message = (char*)malloc(messageLen + bufferSize);
					strcat(message, bytemsg);
					
					//send
					if (send(ClientSock, message, messageLen + bufferSize, 0)<0)
					{
						perror("sending null terminated or given length failed");
						exit(1)
						
					}
					break;

			//3&4
			//apply ntohl to received bytes
				//badInt
				case 3:
				//goodInt
				case 4:
					//ntohl -- ushort int netlong from network byte order to host byte order
					message = (char*)malloc(messageLen + bufferSize);
					strcat(message, ntohl(bytemsg));

					//send
					if (send(ClientSock, message, messageLen + bufferSize, 0)<0)
					{
						perror("sending bad or good int failed");
						exit(1)
					}
					break;


			//5&6
			//reply with commandname and number of recv() operations that server performed to carry it out
				//byteAtATime
				case 5:
					j = 1;
				//kByteAtATime
				case 6:
					if (cmd == 6) j = 1000;

					recvs = 0;
					i = bytemsg; //? idk what to do with number of bytes to send inside of bytemsg
					bytemsg = (char*)malloc(j);

					//receive j bytes at a time

					//send j bytes back...?

					break;

			//never receives noMoreCommands
				//NoMoreCommands
				default:
					break;
			}
			

			//server writes all bytes received into a file
				//write all and only bytes server receives


			//server close client connection when recv = 0 bytes
				//print on console total bytes received on current connection

			//continue calling accept()

		}


	//start reading



	//writing



	}




}
