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





printf("ServerPort is %d\n", ServerPort);





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





printf("Socket made\n");





	//bind socket to server
	if (bind(ServerSock, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))<0)
	{
		perror("bind failed");
		exit(1);
	}






printf("binded\n");





	//listen for clients....(go in sleep mode and wait for incoming connection)
	if (listen(ServerSock, MAXPENDING < 0))
	{
		perror("listen failed");
		exit(1);
	}





printf("listen finished\n");





memset(&ClientAddress, 0, sizeof(ClientAddress));

printf("start of loop\n");

		//set size of the in-out parameter
		ClientLen = sizeof(ClientAddress);


printf("got clientLen: %d\n", ClientLen);
	

	//accept connection from client
		ClientSock = accept(ServerSock, (struct sockaddr *) &ClientAddress, &ClientLen);
		if (ClientSock<0)
		{
			perror("accept failed");
			exit(1);
		}




	//while(1)
	for(;;) //run forever
	{
/*
memset(&ClientAddress, 0, sizeof(ClientAddress));

printf("start of loop\n");

		//set size of the in-out parameter
		ClientLen = sizeof(ClientAddress);


printf("got clientLen: %d\n", ClientLen);
	

	//accept connection from client
		ClientSock = accept(ServerSock, (struct sockaddr *) &ClientAddress, &ClientLen);
		if (ClientSock<0)
		{
			perror("accept failed");
			exit(1);
		}
*/





printf("client accepted\n");
		//client is connected





		//receive message from client
		if ((bytesRcvd = recv(ClientSock, buffer, bufferSize, 0))<0)
		{
			perror("recv failed");
			exit(1);
		}







printf("\nmessage received: %s\n", buffer);
fflush(stdout);





		//send and receive until transmission end

		
		//client message -- first byte = cmd number
		cmd = buffer[0];



//printf("cmd is %d\n", cmd);



		//make message null for next command from client
		message = (char*)malloc(bufferSize);
		memset(message, '\0', bufferSize-1);

		//all sends use command name
		strcat(message, commandNames[cmd-1]);
		strcat(message, ": ");
//printf("commandname in message is %s\n", message);
		messageLen = strlen(message);
//printf("message length is %d\n", messageLen);
		bytemsg = (char*)malloc(bufferSize);
		bytemsg = &buffer[1]; //idk if this will work...
//printf("everything after command name is %s\n", bytemsg);

		//handle commands from client
		switch (cmd)
		{	
		//1-4 handled the same by server
		//reply with commandname: received value
			//nullTerminated
			case 1:
			//givenLength
			case 2:
				strcat(message, bytemsg);
				
				//send
				//HandleClient(ClientSock, message, messageLen+bufferSize, bytesRcvd, "sending null terminated or given length failed");
				if (send(ClientSock, message, bufferSize-1, 0)!=(bufferSize-1))
				{
					perror("sending null terminated or given length failed");
					exit(1);
					
				}





printf("message sent: %s\n", message);
fflush(stdout);


				break;

		//3&4
		//apply ntohl to received bytes
			//badInt
			case 3:
			//goodInt
			case 4:
				//ntohl -- ushort int netlong from network byte order to host byte order
				//message = (char*)malloc(messageLen + bufferSize);
				strcat(message, ntohl(bytemsg));

				//send
				if (send(ClientSock, message, messageLen + bufferSize, 0)<0)
				{
					perror("sending bad or good int failed");
					exit(1);
				}





printf("message sent: %s\n", message);
fflush(stdout);



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
fflush(stdout);
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


	

	//accept connection from client

printf("End of Loop\n");
fflush(stdout);
	}




}








//ClientSock, message, messageLen+bufferSize, "sending null terminated or given length failed"
void HandleClient(int ClientSock, char * message, int length, int bytesRcvd, char * errormsg)
{
	char buffer[bufferSize];
	//int recvSize;

/*
	//recv msg from client
	if((recvSize=recv(ClientSock, buffer, bufferSize,0))<0)
	{
		perror("recv failed");
		exit(1);
	}
*/
	//send recv str and recv until end
	while (bytesRcvd>0) //0 means end of transmission
	{
		if(send(ClientSock, message, length,0)!=length)
		{
			perror(errormsg);
			exit(1);
		}

		if(bytesRcvd = recv(ClientSock, buffer, bufferSize,0)<0)
		{
			perror("recv failed");
			exit(1);
		}
	}

	close(ClientSock);
}
