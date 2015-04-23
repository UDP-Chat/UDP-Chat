/*
 * MessageHistory.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "MessageHistory.h"

MessageHistory::MessageHistory() {
	// TODO Auto-generated constructor stub
	maxMessageId = 0;
	maxPSEQ = 0;
	maxASEQ = 0;
}

MessageHistory::~MessageHistory() {
	// TODO Auto-generated destructor stub
}

ssize_t MessageHistory::get_maxPSEQ(){
	return maxPSEQ;
}
ssize_t MessageHistory::get_maxASEQ(){
	return maxASEQ;
}

Message MessageHistory::createMessage(ssize_t type, string processID, ssize_t messageID, string data){
	Message msg;
	msg.type=type;
	msg.messageId=messageID;
	memcpy(msg.processId,processID.c_str(),processID.length()+1);
	memcpy(msg.data,data.c_str(),data.length()+1);
	return msg;
}
