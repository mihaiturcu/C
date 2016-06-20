/*
 * =====================================================================================
 *
 *       Filename:  Message.h
 *
 *    Description:  Defines our message struct
 *
 *        Version:  1.0
 *        Created:  06/20/16 16:44:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef message_struct_guard
#define message_struct_guard
struct Message {
	int id;
	char body[50];
};
#endif
