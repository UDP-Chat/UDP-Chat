/*
 * MessageHistory.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "Messages.h"
#include "global.h"

Messages::Messages() {
	maxMessageId = 0;
	maxPSEQ = 0;
	maxASEQ = 0;
	sem_init(&lock, 0, 1);
}

Messages::~Messages() {
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

Message Messages::createMessage(Message* message){
	Message m;

	memcpy(m.processId,message->processId,strlen(message->processId));
	memcpy(m.data,message->data,strlen(message->data));
	m.messageId=message->messageId;
	m.type=message->type;
	return m;
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
	bool res=receivedMessages.count(this->getKey(type,processID,messageID))>0;
	sem_post(&lock);
	return res;
}

string Messages::getMessageData(ssize_t type, string processID, ssize_t messageID){
	sem_wait(&lock);
	string res=receivedMessages.find(this->getKey(type,processID,messageID))->second;
	sem_post(&lock);
	return res;
}


bool Messages::checkout(Message m){
	if(this->existMessage(m)){
		return true;
	}else{
		this->putMessage(m);
		return false;
	}
}
bool Messages::checkout(Message* m){
	return this->checkout(this->createMessage(m));
}

void Messages::putMessage(Message m){
	Message2 m2=this->convert_message_to_cpp(m);
	sem_wait(&lock);
	pair<string, string> p(this->getKey(m2.type,m2.processId,m2.messageId),m2.data);
	receivedMessages.insert(p);
	sem_post(&lock);
}


string Messages::getKey(ssize_t type, string processId, ssize_t messageId){
	return to_string(type)+"#"+processId+"#"+to_string(messageId);
}









bool Messages::sendJOIN(string group_processId){
	Message msg=createMessage(TYPE_JOIN, udp->processID, MESSAGE_ID_JOIN,udp->name);

	Message expectedReturn=createMessage(TYPE_LIST, group_processId, MESSAGE_ID_LIST,"");

	int trials=0;

	while(this->existMessage(expectedReturn)==false && trials<MAX_TRAILS){
		cout << "   sending JOIN..." << endl;
		udp->send_msg(group_processId,msg);
		trials++;
		std::this_thread::sleep_for (std::chrono::seconds(RESEND_INTERVAL));
	}

	if(this->existMessage(expectedReturn)){
		return true;
	}else{
		return false;
	}
}


void Messages::sendNEW(){
	this->sendFourway(TYPE_NEW, MESSAGE_ID_NEW, udp->name, "   sending NEW to ");

	//TODO finish up mark able to start etc think
}


void Messages::sendASEQ(ssize_t messageID, int aseq){
	int n=members->memberList.size();
	thread threads[n];

	int i=0;

	Message msg=this->createMessage(TYPE_ASEQ, udp->processID, MESSAGE_ID_NEW, to_string(aseq));

	// send ASEQ message to all the members in the group
	for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
		string pid=(*it).first;
		Message expectedReply=createMessage(TYPE_ACK, pid, MESSAGE_ID_NEW,"");
		threads[i]=std::thread(&Messages::sendMessageTimeoutTo, this, pid, msg, expectedReply, "   sending ASEQ to "+pid+" ...");
		i++;
	}

	for(int j=0; j<i;j++){
		threads[j].join();
	}
}


void Messages::sendMessageTimeoutTo(string processID, Message message, Message expectedReply, string printString){
	int trials=0;

	while(this->existMessage(expectedReply)==false && trials<MAX_TRAILS){
		cout << printString << endl;
		udp->send_msg(processID,message);
		trials++;
		std::this_thread::sleep_for (std::chrono::seconds(RESEND_INTERVAL));
	}

	if(this->existMessage(expectedReply)==false){
		members->reportDie(processID);
	}
}


void Messages::sendACK(string processID, Message m){
	udp->send_msg(processID,m);
}


void Messages::sendDATA(string content){
	int mid=maxMessageId;
	maxMessageId++;

	this->sendFourway(TYPE_DATA, mid, content, "   sending Chat DATA to ");
}

void Messages::sendFourway(ssize_t type, ssize_t messageID, string data, string printString){
	int n=members->memberList.size();
	thread threads[n];


	Message msg=this->createMessage(type, udp->processID, messageID, data);

	int i=0;
	// send DATA message to all the members in the group
	for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
		string pid=(*it).first;
		Message expectedReply=createMessage(TYPE_PSEQ, pid, messageID,"");
		threads[i]=std::thread(&Messages::sendMessageTimeoutTo, this, pid, msg, expectedReply, printString+pid+" ...");
		i++;
	}

	for(int j=0; j<i;j++){
		threads[j].join();
	}


	//get max PSEQ from members, and prepare to send ASEQ
	int maxPSEQ=-1;
	for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
		string pid=(*it).first;
		int pseq=atoi(messageStore->getMessageData(TYPE_PSEQ, pid, messageID).c_str());
		if(pseq>maxPSEQ)maxPSEQ=pseq;
	}

	//TODO update local max ASEQ

	messageStore->sendASEQ(messageID, maxPSEQ);
}


