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

typedef struct{
	Message m;
	bool deliverable;
	ssize_t Seq;
} HoldBackQueueItem;

#endif /* MESSAGE_FORMAT_H_ */
