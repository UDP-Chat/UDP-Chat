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

string MessageHistory::getIP(string ip_port){
	std::string delimiter = ":";
	std::string ip = ip_port.substr(0, ip_port.find(delimiter));
	return ip;
}

string MessageHistory::getPort(string ip_port){
	std::string ip = ip_port.substr(getIP(ip_port).length()+1);
	return ip;
}

Message2 MessageHistory::convert_message_to_cpp(Message m){
	Message2 res;
	res.type=m.type;
	res.processId=string(m.processId);
	res.processIP=getIP(res.processId);
	res.processPort=getPort(res.processId);
	res.messageId=m.messageId;
	res.data=string(m.data);
	return res;
}


bool MessageHistory::existMessage(Message m){
	return this->existMessage(this->convert_message_to_cpp(m));
}

bool MessageHistory::existMessage(Message2 m){
	return this->existMessage(m.type, m.processId, m.messageId);
}


bool MessageHistory::existMessage(ssize_t type, string processID, ssize_t messageID){
	return receivedMessages.count(to_string(type)+processID+messageID)>0;
}

string MessageHistory::getMessageData(ssize_t type, string processID, ssize_t messageID){
	return receivedMessages.find(to_string(type)+processID+messageID)->second;
}
