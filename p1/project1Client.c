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

memset(buffer, '\0', bufferSize-1);
//strcat(buffer, "is it even writing?\n");
//printf(buffer);

	//struct command c;

	//struct variable for for sockaddr_in servaddr - server address
	struct sockaddr_in ServerAddress;





//argv[]
for(i=0;i<argc;i++)
{
	printf("argv[%d]=%s\n", i, argv[i]);
}




	//need four or three arguments? maybe not for this program...
	if (argc != 3)
	{
		exit(1);
	}

	serverIP = argv[1];
	serverport = atoi(argv[2]); //use given port

	//create a socket -- create stream socket using tcp
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //create socket point
	//socket() returned bad number
	if (sock< 0)
	{
		printf("socket() failed");
		exit(1);
	}





printf("Socket made\n");




	//server = gethostbyname(argv[1]); //?? code in tutorial, need another struct for host
	//checking for server == NULL --> no host error if it does

	memset(&ServerAddress, 0, sizeof(ServerAddress)); //zero out structure
	ServerAddress.sin_family = AF_INET; //internet address family






//printf("0this program exists\n");
printf("serverport = %d\n", serverport);
printf("serverip = %s\n", serverIP);




	
	ServerAddress.sin_addr.s_addr = inet_addr(serverIP); //server ip address
//printf("1this program exists\n");
	ServerAddress.sin_port = htons(serverport); //server port
//printf("2this program exists\n");

	//connect to server
	if (connect(sock, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
	{
		perror("Error connecting");
		exit(1);
	} 






printf("Connection made\n\n");




//printf("3this program exists\n");

	//commands
	for (i=0;i<3;i++)
	{

printf("4this program exists -- start of for loop\n");
		message = (char*)malloc(bufferSize+1);
		//put command number into message
		//strcpy(message, (char*)(i+1));
		message[0]=(i+1);
//printf("5this program exists\n");

printf("%d\n",i);
		//put bytes into message
		switch (i)
		{
			case 0:
			case 1:
//printf("does strcat message commands i arg work?\n");
				strcat(message, commands[i].arg);
printf("yes message is %s\n", message);
				break;
			case 2:
//printf("does cmd = atoi(commands[i].arg) work?\n");
				//bytemsg = (char*)malloc(sizeof(commands[i].arg)+1);
				cmd = atoi(commands[i].arg);
printf("yes, it's %d but does strcat message, cmd?\n", cmd);

				message = (char*)malloc(sizeof(commandNames[i])+sizeof(commands[i].arg)+1);
				strcat(message, cmd);
//printf("yes, it's %s\n", message);
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

printf("6this program exists -- after command switch\n");
		
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
					perror("client send byte at time failed\n");
					exit(1);
				}
				sends++;

				//receive j bytes into bytesRcvd
				bytesRcvd = recv(sock, buffer, bufferSize-1, 0);
				if (bytesRcvd < 0)
				{
					perror("client receive byte at time failed\n");
					exit(1);
				}
				
				//decrement cmd
				cmd = cmd - bytesRcvd;
				
			}

		}
		else
		{




printf("got into the send() part, sending %s to socket %d\n", message, sock);



			//send message
			messageLen = strlen(message);
			if (send(sock, message, messageLen, 0) < 0)
			{
				perror("client send failed");
				exit(1);
			}




printf("sent it. got to the recv() part\n");
printf("buffer before recv(): %s\n", buffer);



			//receive response
			memset(buffer, '\0', bufferSize-1);
			
			if ((bytesRcvd = recv(sock, buffer, bufferSize-1, 0))< 0)
			{
				perror("client recv() failed\n");
				exit(1);
			}
printf("bytes rcd = %d from socket %d\n", bytesRcvd, sock);
			//bytesRcvd = recv(sock, buffer, bufferSize-1, 0);
			//buffer[bytesRcvd] = '\0';
		}
		//printf("got to the print buffer part, but buffer is not printing\n");
		//print response on stdout
//printf("received into the buffer: %s\n", buffer);
		printf(buffer);
		printf("\n\n");			

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
