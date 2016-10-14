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
	int sendnum;
	int j, cmd, recvs;
	int gotcmd = 0;

	int opensocket = 0; //0 for closed, 1 for connected

	//need 2 arguments
	if (argc != 2)
	{
		exit(1);
	}

	ServerPort = atoi(argv[1]); //second arg: local port





//printf("ServerPort is %d\n", ServerPort);





	//call socket(), make socket
	if ((ServerSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
	{
		perror("socket failed");
		exit(1);
	}

	//set up server details
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





	//while(1)
	for(;;) //run forever
	{
		memset(&ClientAddress, 0, sizeof(ClientAddress));

		//set size of the in-out parameter
		ClientLen = sizeof(ClientAddress);
	
		//accept connection from client
		ClientSock = accept(ServerSock, (struct sockaddr *) &ClientAddress, &ClientLen);
		if (ClientSock<0)
		{
			perror("accept failed");
			exit(1);
		}

		//client is connected
		opensocket = 1;





printf("client accepted\n\n");




	
		//while client is connected
		while (opensocket == 1)
		{	
printf("start of while, %s\n", buffer);
			//receive message from client
			if ((bytesRcvd = recv(ClientSock, buffer, bufferSize, 0))<0)
			{
				perror("recv failed");
				exit(1);
			}





printf("\nmessage received from client: %s\n", buffer);





			//send and receive until transmission end

		
			//client message -- first byte = cmd id number
			cmd = buffer[0];

			//make message null for next command from client
			message = (char*)malloc(bufferSize);
			memset(message, '\0', bufferSize);

			//all sends use command name
			strcat(message, commandNames[cmd-1]);
			strcat(message, ": ");
			messageLen = strlen(message);

			//have bytemsg point to everything after cmd id number, if there is anything after
			bytemsg = (char*)malloc(bufferSize);
			bytemsg = &buffer[1];


			//handle commands from client
			switch (cmd)
			{	

			//1-4 handled the same by server
			//reply with commandname: received value

				//nullTerminated
				case 1:
				//givenLength
				case 2:
					//combine command name and the command arg
					strcat(message, bytemsg);
				
					//send both the Command Name and the Command.Arg for case 1 or case 2
					if (send(ClientSock, message, bufferSize-1, 0)!=(bufferSize-1))
					{
						perror("sending null terminated or given length failed");
						exit(1);
					}





printf("message sent to client: %s\n", message);

					break;





			//3&4
			//apply ntohl to received bytes

				//badInt
				case 3:
				//goodInt
				case 4:



printf("sending %s\n", message);


					//send the Command Name
					if (send(ClientSock, message, messageLen, 0)<0)
					{
						perror("sending bad or good int failed");
						exit(1);
					}


printf("sent it. recv()\n");


					//receive good or bad integer
					if ((bytesRcvd = recv(ClientSock, &sendnum, sizeof(long), 0)) < 0)
					{
						perror("receiving bad or good int failed");
						exit(1);
					}

					//ntohl -- ushort int netlong from network byte order to host byte order
					sendnum = ntohl(sendnum);
printf("sendnum %d\n", sendnum);

					bytemsg = &buffer[0];
					//convert sendnum to ascii string
					strint(sendnum, bytemsg);
printf("after strint\n");
					//send sendnum string
					if (send(ClientSock, bytemsg, strlen(bytemsg), 0)<0)
					{
						perror("sending bad or good int failed");
						exit(1);
					}

printf("message sent to client: %s%s\n", message, bytemsg);

					break;





			//5&6
			//reply with commandname and number of recv() operations that server performed to carry it out
			
				//byteAtATime
				case 5:
					j = 1; //for byte by byte
				//kByteAtATime
				case 6:
					if (cmd == 6) j = 1000; //for 1k bytes by 1k bytes

					recvs = 0; //number of receives so far
					//i = bytemsg; //? idk what to do with number of bytes to send inside of bytemsg
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

		memset(buffer, 0, bufferSize);
		message = (char *)malloc(1);
		bytemsg = (char *)malloc(1);
printf("End of while\n");	
		}

	//accept connection from client
	//the accept happens at the top of for loop
printf("end of for\n");
	}
}



void strint(int num, char *str)
{
	char *n[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	char *str1, *str2;
	int remainder;
	int i = 0;
	int neg = 0;
	int base = 10;

	str1 = (char *)malloc(bufferSize);
	strcpy(str1, "");

	if (num == 0) 
	{
		strcpy(str, "0");
		return;
	}

	if (num < 0)
	{
		i++;
		neg = 1;
		num *= -1;
	}



printf("strint %d\n", num);


	while (num >= 1)
	{
		remainder = num % base;
		
		if (remainder == 0)
		{
			if (num >= base)
			{
				strcpy(str2, "0");
			}
			
		}
		else if (remainder < 10)
		{
printf("n[remainder] %s\n", n[remainder]);
			strcpy(str2, n[remainder]);
		}

		strcat(str1, str2);
		num -= remainder;
		num /= base;
	}

	memset(str, ' ', strlen(str1));

	if (neg !=0)
	{
		str[0] = '-';
	}
	
	for (i; i<strlen(str1); i++)
	{
printf("str1[i]: %c\n", str1[i]);
		str[strlen(str1)+neg-1-i] = str1[i];
printf("str %s\n\n", str);

	}
printf("strint num in strint: %s\n", str);
}
