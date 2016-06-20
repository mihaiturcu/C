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
int SendTo(int port){
	// magically sends a message to another process :), helps with the broadcast method
}
int 
BindSocketInNewProc(int port)
{
	int		sockfd    , newsockfd, clilen;
	struct sockaddr_in cli_addr, serv_addr;
	char		string    [MAX_SIZE];
	int		len;
	char buffer[200];
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
			//printf("%s\n", &string);
			// elaborate logging technique below -->
			unsigned long uid = fprintf(stdout, "%lu\n", (unsigned long)time(NULL));
			snprintf(buffer, sizeof(buffer), "echo \"Process with almost unique identifier:[%lu] recieved this: %s\" >> log",&uid,&string);
			int writeToFile=system(buffer);
			close(newsockfd);
		}

	} else {
		return 0;
	}
}
