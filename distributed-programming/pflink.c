/*
 * 
 * ============================================================================
 * =========
 * 
 * Filename:  main.c
 * 
 * Description:
 * 
 * Version:  1.0 Created:  06/19/16 23:50:57 Revision:  none Compiler:  gcc
 * 
 * =====================================
 * ===============================================
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "pflink.h"
#define MAX_SIZE 80
// note to self: don't send structs, requires serialization == bad stuff
int StartsWith(const char *a,const char *b)
{
	//if(StartsWith("http://stackoverflow.com", "http://")) just sampling
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

void logToSomeFile(char buffer[200],int fileChoice){
	char ToLog[] = ">> log";
	char ToDebug[] = ">> debug";
	char ToError[] = ">> error";
	char ToOpenPorts[] = ">> availablePorts";
		// logging every used port so i know who to broadcast to :)
	char newbuff[250];
	memset(newbuff,0,250);
	strcat(newbuff,buffer);
	printf("First strcat: %s",&newbuff);
	if(fileChoice==1){
		strcat(newbuff,ToLog);
	}
	else if(fileChoice==2){
		strcat(newbuff,ToDebug);
	}
	else if(fileChoice==3){
		strcat(newbuff,ToError);
	}
	else if(fileChoice==4){
		strcat(newbuff,ToOpenPorts);
	}
	printf("Before system: %s",&newbuff);
	int ignoringThis=system(newbuff);
}

int SendTo(char message[200], int port){
	// magically sends a message to another process :), helps with the broadcast method
	struct sockaddr_in serv_addr;
	int sockfd = 0;
	int clientsockfd = socket(AF_INET, SOCK_STREAM, 0);
  	if (sockfd < 0) {
      perror("Client: ERROR opening socket");
      exit(1);
	   }
	memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
	logToSomeFile("Just testing this if it works",2);
	//hardcoded to send to localhost could add this to params at some later time
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        logToSomeFile("Client inet_pton error occured",3);
        return 1;
    }
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       logToSomeFile("Client Error : Connect Failed",3);
       return 1;
    }


}
int BindSocketInNewProc(int port)
{
	int		sockfd    , newsockfd, clilen;
	struct sockaddr_in cli_addr, serv_addr;
	char		string    [MAX_SIZE];
	int		len;
	char cleanbuffer[300];
	memset(cleanbuffer,0,300);
	//itoa(port,portToString,10);
	//printf("return code: %d ",c);
	//printf("%s -- stringy",&portToString);
	//will log to file, someday
	pid_t pid = fork();
	if (pid == -1) {
		printf("Fork fail");
		exit(2);
	} else if (pid == 0) {
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("can't open stream socket");
			exit(1);
		}
		/*
		 * bind the local address, so that the cliend can send to
		 * server
		 */
		bzero((char *)&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(port);
		if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			perror("can't bind local address");
			exit(1);
		}
		/* listen to the socket */
		listen(sockfd, 5);
//		bzero((char *)&cleanbuffer, sizeof(cleanbuffer));
		for (;;) {
			/*
			 * wait for a connection from a client; this is an
			 * iterative server
			 */

			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
			if (newsockfd < 0) {
				perror("can't bind local address");
			}
			/* read a message from the client */
			len = read(newsockfd,string, MAX_SIZE);
			/* make sure it's a proper string */
			string[len] = 0;
//			printf("issue\n");
//			printf("%s",&string);
			//strcat(string,"");
			if(StartsWith(string,"broadcast"))
				logToSomeFile("Yep, got a message starting with broadcast",2);
			//printf("%s\n", &string);
			//
			// elaborate logging technique below -->
			snprintf(cleanbuffer, sizeof(cleanbuffer),"echo 'Process with almost unique identifier:[notImplementedYet] recieved this: %s'",&string);
			printf("[%s]",&cleanbuffer);
			logToSomeFile(cleanbuffer,1);
			//int writeToFile=system(buffer);
			close(newsockfd);
		}

	} else {
		return 0;
	}
}
