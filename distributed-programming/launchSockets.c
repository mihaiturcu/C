/*
 * =====================================================================================
 *
 *       Filename:  launchSockets.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/20/16 12:42:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "pflink.h"
int main(int argc, char* argv[]){
	// this will launch a socket and bind it to port in first arg
	BindSocketInNewProc(atoi(argv[1]));
}

