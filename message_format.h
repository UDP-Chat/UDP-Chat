/*
 * message_format.h
 *
 *  Created on: Apr 14, 2015
 *      Author: cis455
 */

#ifndef MESSAGE_FORMAT_H_
#define MESSAGE_FORMAT_H_
#include <string>


typedef struct {
	ssize_t type;
	char processId[22];
	ssize_t messageId;
	char data[10240];
} Message;

typedef struct {
	ssize_t type;
	string processId;
	ssize_t messageId;
	string data;
	string key;
} Message2;

typedef struct{
	Message2 m;
	bool deliverable;
	ssize_t Seq;
} HoldBackQueueItem;

typedef struct{
	long time;
	string name;
} MemberInfo;

#endif /* MESSAGE_FORMAT_H_ */
