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

	char *message, *bytemsg; //string to send to server
	char buffer[bufferSize]; //buffer for message received
	unsigned int messageLen; //length of message

	int bytesRcvd; //bytes read in single recv()
	int totalBytesRcvd; //total bytes read
	int i, j, cmd, sends;

	//struct variable for for sockaddr_in servaddr - server address
	SockAddr_In ServerAddress;

	//need four or three arguments? maybe not for this program...
	if (argc < 3 || argc > 4)
	{
		exit(1);
	}

	if (argc == 4)
		serverPort = atoi(argv[3]); //use given port
	else
		serverPort = 7; //7 is well known port for echo service


	//create a socket -- create stream socket using tcp
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //create socket point

	//socket() returned bad number
	if (sock< 0)
	{
		printf("socket() failed");
		exit(1);
	}

	//server = gethostbyname(argv[1]); //?? code in tutorial, need another struct for host
	//checking for server == NULL --> no host error if it does


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


	//commands
	for (i=0;i<6;i++)
	{
		//put command number into message
		strcopy(message, commands[i].cmd);

		//put bytes into message
		switch (i)
		{
			case 0:
			case 1:
				strcat(message, commands[i].arg);
				break;
			case 2:
				cmd = atoi(commands[i].arg);
				strcat(message, cmd);
				break;
			case 3:
			case 4:
				j = 1;
			case 5:
				if (i==5) j = 1000;
				cmd = atoi(commands[i].arg);
				strcat(message, htonl(cmd));
				break;

			default:
				//don't send anything
				//exit loop
				//close socket
				if (close(sock)<0)
				{
					perror("client close socket failed");
					exit(1);
				}
				else
				{
					exit(0);
				}
				break;
		}
		
		//send and receive message
		if (i == 4 || i == 5)
		{
			sends=0;
			//first send is command number and number of bytes to send
			if (send(sock, message, messageLen, 0) < 0)
			{
				perror("client send byte at time failed");
				exit(1);
			}

			//still have bytes left to send
			while (cmd > 0)
			{
				//send j bytes of alternating 0 and 1
				memset(bytemsg, (char)((sends%2)*1), j);
				if (send(sock, bytemsg, j, 0)<0)
				{
					perror("client send byte at time failed");
					exit(1);
				}
				send++;

				//receive j bytes into bytesRcvd
				bytesRcvd = recv(sock, buffer, bufferSize-1, 0);
				if (bytesRcvd < 0)
				{
					perror("client receive byte at time failed");
					exit(1);
				}
				
				//decrement cmd
				cmd = cmd - bytesRcvd;
				
			}

		}
		else
		{
			//send message
			messageLen = strLen(message);
			if (send(sock, message, messageLen, 0) < 0)
			{
				perror("client send failed");
				exit(1);
			}

			//receive response
			if ((bytesRcvd = recv(sock, buffer, bufferSize-1, 0))< 0)
			{
				perror("client recv() failed");
				exit(1);
			}
		}

		//print response on stdout
		printf(buffer);				

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
}
