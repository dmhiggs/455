/*
	socket()
	connect()
	write()
	read()
	close()
*/
//got info from Unix Socket Quick Guide on tutorialspoint.com
//127.0.0.1 points to self

#include "project1.h"

int main(int argc, char *argv[])
{
	//variables for socket descriptor, portnumber
	//server address of server socket
	//buffer
	//some sort of variable for server???
	//others???

	int sock; //socket descriptor
	unsigned short serverport; //server port
	char *serverIP; //server IP address

	char *message; //string to send to server
	char buffer[bufferSize]; //buffer for message received

	int bytesRcvd; //bytes read in single recv()
	int i, cmd, arg; //int for counter, int for command id number, int for atoi(command arg)


	//struct variable for for sockaddr_in servaddr - server address
	struct sockaddr_in ServerAddress;

	//need three arguments
	if (argc != 3)
	{
		exit(1);
	}

	//argv assignments
	serverIP = argv[1]; //set the ip
	serverport = atoi(argv[2]); //use given port

	//create a socket -- create stream socket using tcp
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //create socket point

	//socket() returned bad number
	if (sock< 0)
	{
		printf("socket() failed");
		exit(1);
	}





//printf("Socket made\n");





	//set up server
	memset(&ServerAddress, 0, sizeof(ServerAddress)); //zero out structure
	ServerAddress.sin_family = AF_INET; //internet address family
	ServerAddress.sin_addr.s_addr = inet_addr(serverIP); //server ip address
	ServerAddress.sin_port = htons(serverport); //server port

	//connect to server
	if (connect(sock, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
	{
		perror("Error connecting");
		exit(1);
	} 





//printf("Connection made\n\n");




printf("\n");
	//commands
	for (i=0;i<7;i++)
	{
		//resize message to int size
		message = (char*)malloc(bufferSize);

		//cmd id number
		cmd = i + 1;

		//put command number into message
		message[0]=(cmd);

		//put bytes into message
		switch (i)
		{
			//null terminated & given length -- sends strings (cmd and arg)
			case 0:
				//resize message
				message = (char*)malloc(bufferSize+1);

				//put command number into message
				message[0]=(cmd);

			case 1:
				//add the command arg str to the cmd number
				strcat(message, commands[i].arg);
				if (i==0) message[bufferSize]='\0'; //add null terminator for case 0

				//send and receive from server
				sendtwostr(message, &buffer, sock, &bytesRcvd);
				break;




			//bad int -- sends string and number (cmd and integer)
			case 2:
				//resize message
				//message = (char*)malloc(bufferSize);

				//convert Command Arg to an int
				arg = atoi(commands[i].arg);


//printf(" \n"); //for some reason it doesn't work without this line??????????????

				//send the cmd id number in string and the cmd arg number as int
				sendstrnum(message, &buffer, sock, &bytesRcvd, arg);
				break;




			//good int, byte at a time, kbyte at a time
			case 3: //sends cmd and integer
			case 4: //sends numbers in bytes
			case 5: //sends numbers in bytes

				//convert Command Arg to an int
				arg = atoi(commands[i].arg);

				//apply htonl() to int (host to network long)
				arg = htonl(arg);

//printf("\0"); //for some reason it doesn't work without this line??????????????
				
				//send and receive from servers
				if (i==3) sendstrnum(message, &buffer, sock, &bytesRcvd, arg);
				if (i==4) sendstrbytes(message, &buffer, sock, &bytesRcvd, arg, 1);
				if (i==5) sendstrbytes(message, &buffer, sock, &bytesRcvd, arg, 1000);

				break;




			//treating this as the No Command command
			default:
				//don't send anything
				//exit loop
				//close socket
/*
				if (send(sock, 0, 0, 0)!=0)
				{
					perror("send 0 failed");
					exit(1);
				}
*/
shutdown(sock, SHUT_WR);
printf("\n");
//exit(0);
return 0;
/*
				if (close(sock)<0)
				{
					perror("client close socket failed");
					exit(1);
				}
				else
				{
					exit(0);
				}
*/
				break;
		}//end switch

if( i!=6){
		if (send(sock, "1", 1, 0)<0)
		{
			perror("send to restart loop failed");
			exit(1);
		}}

		//print response on stdout
		printf(buffer);
		printf("\n");			
		
		//empty buffer???	
		memset(buffer, 0, bufferSize);
		message = (char *)malloc(1);
	}//end for

}//end main






//send two strings, receive 1
void sendtwostr(char* message, char* buffer, int sock, int *bytesRcvd)
{
	int messageLen;

	memset(buffer, '\0', bufferSize-1);

	//send message
	messageLen = strlen(message);
	if (send(sock, message, messageLen, 0) < 0)
	{
		perror("client send failed");
		exit(1);
	}
	
	//receive response
	if ((bytesRcvd = recv(sock, buffer, bufferSize-1, 0))< 0)
	{
		perror("client recv() failed\n");
		exit(1);
	}
}





//send string and number, receive two strings
void sendstrnum(char* message, char* buffer, int sock, int *bytesRcvd, int arg)
{
	int messageLen;
	char *bufpointer;

	memset(buffer, '\0', bufferSize-1);
	//memset(bufpointer, '\0', bufferSize-1);


	//send message containing cmd id number
	messageLen = strlen(message);
	if (send(sock, message, messageLen, 0) < 0)
	{
		perror("send failed");
		exit(1);
	}


	//receive Command name from server
	if ((bytesRcvd = recv(sock, buffer, bufferSize-1, 0)) < 0)
	{
		perror("receive failed");
		exit(1);
	}


	//point to where the message ended in buffer
	bufpointer = &buffer[(int)bytesRcvd-1];


	//send integer to server
	if (send(sock, &arg, sizeof(long), 0) < 0)
	{
		perror("send failed");
		exit(1);
	}




	//receive string from server into pointer into buffer where Command Name ended
	if ((bytesRcvd = recv(sock, bufpointer, bufferSize-1, 0)) < 0)
	{
		perror("receive failed");
		exit(1);
	}

}





//send string and J amount of bytes multiple times, receive string and J amount of bytes multiple times
void sendstrbytes(char* message, char* buffer, int sock, int* bytesRcvd, int arg, int j)
{
	int messageLen;
	int count = 0;
	char* bufpointer;
	char temp[bufferSize];
	
	int *bytes;

	//memset(bufpointer, '\0', bufferSize-1);


	//first send is message containing cmd id number
	messageLen = strlen(message);
	if (send(sock, message, messageLen, 0) < 0)
	{
		perror("client send byte at time failed");
		exit(1);
	}

	//receive Command Name from server
	if ((bytesRcvd = recv(sock, buffer, bufferSize, 0)) < 0)
	{
		perror("receive failed");
		exit(1);
	}


	//point to where the message ended in buffer
	//bufpointer = &buffer[strlen(buffer)-1];

	//send integer to server
	if (send(sock, &arg, sizeof(arg), 0) < 0)
	{
		perror("send failed");
		exit(1);
	}

	//change size of message to j
	message = (char*)malloc(j);
	//bytes = (int *)malloc(j);

	//change arg back to what it was...
	arg = ntohl(arg);

	//send and receive a bunch of blocks (alternating 0 & 1) of j size until arg is empty
	while (arg > 0) //decrement by j?
	{
		//set message to all 0s or all 1s
		if (count%2 == 0) memset(message, '0', j);
		else memset(message, '1', j);

		//send jblock to server
		if (send(sock, message, j, 0) < 0)
		{
			perror("client send byte at time failed\n");
			exit(1);
		}

		
		//decrement arg
		arg = arg - j;

		//increment count
		count++;

		//empty temp?
	}

//bufpointer = (char*)malloc(bufferSize);
//memset(bufpointer, '\0', bufferSize);

	//receive number of server receives into temp
	bytesRcvd = recv(sock, temp, bufferSize, 0); //if statement sends error even when it doesn't fail...?
/*
	if ((bytesRcvd = recv(sock, temp, bufferSize, 0)) < 0);
	{
		perror("client receive byte at time failed\n");
		printf("%s\n%d\n", temp, bytesRcvd);
		exit(1);
	}
*/
strcat(buffer, temp);
}


















/*
	//read server response -- get same string back from server
	totalBytesRcvd = 0;
	printf("Received: ");
	
	while (totalBytesRcvd < messageLen)
	{
		//receive up to buffer size - 1 
		//from sender
		if ((bytesRcvd = recv(sock, buffer, bufferSize-1, 0))<=0)
		{
			perror("recv() failed or connection closed early");
			exit(1)
		}
		
		totalBytesRcvd += bytesRcvd; //increment total bytes
		buffer[bytesRcvd] = '\0'; //terminate string
		printf(buffer); //print buffer
	}
*/
