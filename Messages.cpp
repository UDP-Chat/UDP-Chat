/*
 * MessageHistory.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "Messages.h"

Messages::Messages() {
	// TODO Auto-generated constructor stub
	maxMessageId = 0;
	maxPSEQ = 0;
	maxASEQ = 0;
	sem_init(&lock, 0, 1);
}

Messages::~Messages() {
	// TODO Auto-generated destructor stub
}

ssize_t Messages::get_maxPSEQ(){
	return maxPSEQ;
}
ssize_t Messages::get_maxASEQ(){
	return maxASEQ;
}

Message Messages::createMessage(ssize_t type, string processID, ssize_t messageID, string data){
	Message msg;
	msg.type=type;
	msg.messageId=messageID;
	memcpy(msg.processId,processID.c_str(),processID.length()+1);
	memcpy(msg.data,data.c_str(),data.length()+1);
	return msg;
}

string Messages::getIP(string ip_port){
	std::string delimiter = ":";
	std::string ip = ip_port.substr(0, ip_port.find(delimiter));
	return ip;
}

string Messages::getPort(string ip_port){
	std::string ip = ip_port.substr(getIP(ip_port).length()+1);
	return ip;
}

Message2 Messages::convert_message_to_cpp(Message m){
	Message2 res;
	res.type=m.type;
	res.processId=string(m.processId);
	res.processIP=getIP(res.processId);
	res.processPort=getPort(res.processId);
	res.messageId=m.messageId;
	res.data=string(m.data);
	return res;
}


bool Messages::existMessage(Message m){
	return this->existMessage(this->convert_message_to_cpp(m));
}

bool Messages::existMessage(Message2 m){
	return this->existMessage(m.type, m.processId, m.messageId);
}


bool Messages::existMessage(ssize_t type, string processID, ssize_t messageID){
	sem_wait(&lock);
	bool res=receivedMessages.count(to_string(type)+processID+to_string(messageID))>0;
	sem_post(&lock);
	return res;
}

string Messages::getMessageData(ssize_t type, string processID, ssize_t messageID){
	sem_wait(&lock);
	string res=receivedMessages.find(to_string(type)+processID+to_string(messageID))->second;
	sem_post(&lock);
	return res;
}


void Messages::putMessage(Message m){
	Message2 m2=this->convert_message_to_cpp(m);
	sem_wait(&lock);
	pair<string, string> p(to_string(m2.type)+m2.processId+to_string(m2.messageId),m2.data);
	receivedMessages.insert(p);
	sem_post(&lock);
}
