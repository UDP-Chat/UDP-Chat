/*
 * MessageHistory.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "MessageStore.h"

#include "global.h"

MessageStore::MessageStore() {
	maxMessageId = 0;
	maxPSEQ = 0;
	maxASEQ = 0;
	sem_init(&lock, 0, 1);
}

MessageStore::~MessageStore() {
}

ssize_t MessageStore::get_maxPSEQ(){
	return maxPSEQ;
}
ssize_t MessageStore::get_maxASEQ(){
	return maxASEQ;
}
void MessageStore::set_maxPSEQ(ssize_t n){
	maxPSEQ = n;
}
void MessageStore::set_maxASEQ(ssize_t n){
	maxASEQ = n;
}

Message MessageStore::createMessage(ssize_t type, string processID, ssize_t messageID, string data){
	Message msg;
	msg.type=type;
	msg.messageId=messageID;
	memcpy(msg.processId,processID.c_str(),processID.length()+1);
	memcpy(msg.data,data.c_str(),data.length()+1);
	return msg;
}

Message MessageStore::createMessage(Message* message){
	Message m;

	memcpy(m.processId,message->processId,strlen(message->processId)+1);
	memcpy(m.data,message->data,strlen(message->data)+1);
	m.messageId=message->messageId;
	m.type=message->type;
	return m;
}

string MessageStore::getIP(string ip_port){
	std::string delimiter = ":";
	std::string ip = ip_port.substr(0, ip_port.find(delimiter));
	return ip;
}

string MessageStore::getPort(string ip_port){
	std::string ip = ip_port.substr(getIP(ip_port).length()+1);
	return ip;
}

Message2 MessageStore::convert_message_to_cpp(Message m){
	Message2 res;
	res.type=m.type;
	res.processId=string(m.processId);
	res.processIP=getIP(res.processId);
	res.processPort=getPort(res.processId);
	res.messageId=m.messageId;
	res.data=string(m.data);
	return res;
}


bool MessageStore::existMessage(Message m){
	return this->existMessage(this->convert_message_to_cpp(m));
}

bool MessageStore::existMessage(Message2 m){
	return this->existMessage(m.type, m.processId, m.messageId);
}


bool MessageStore::existMessage(ssize_t type, string processID, ssize_t messageID){
	sem_wait(&lock);
	bool res=receivedMessages.count(this->getKey(type,processID,messageID))>0;
	sem_post(&lock);
	return res;
}

string MessageStore::getMessageData(ssize_t type, string processID, ssize_t messageID){
	sem_wait(&lock);
	string res=receivedMessages.find(this->getKey(type,processID,messageID))->second;
	sem_post(&lock);
	return res;
}


bool MessageStore::checkout(Message2 m){
	if(this->existMessage(m)){
		return true;
	}else{
		this->putMessage(m);
		return false;
	}
}
bool MessageStore::checkout(Message* m){
	return this->checkout(this->convert_message_to_cpp(this->createMessage(m)));
}

void MessageStore::putMessage(Message2 m2){
	sem_wait(&lock);
	pair<string, string> p(this->getKey(m2.type,m2.processId,m2.messageId),m2.data);
	receivedMessages.insert(p);
	sem_post(&lock);
}


string MessageStore::getKey(ssize_t type, string processId, ssize_t messageId){
	return std::to_string(type)+"#"+processId+"#"+std::to_string(messageId);
}









bool MessageStore::sendJOIN(string group_processId){
	Message msg=createMessage(TYPE_JOIN, udp->processID, MESSAGE_ID_JOIN,udp->name);

	Message expectedReturn=createMessage(TYPE_LIST, group_processId, MESSAGE_ID_LIST,"");

	int trials=0;

	while(this->existMessage(expectedReturn)==false && trials<MAX_TRAILS){
//		cout << "   sending JOIN..." << endl;
		udp->send_msg(group_processId,msg);
		trials++;
		std::this_thread::sleep_for (std::chrono::milliseconds(RESEND_INTERVAL));
	}

	if(this->existMessage(expectedReturn)){
		return true;
	}else{
		return false;
	}
}


void MessageStore::sendNEW(){
	this->sendFourway(TYPE_NEW, MESSAGE_ID_NEW, udp->name, "   sending NEW to ");
}


void MessageStore::sendASEQ(ssize_t messageID, int aseq){
	int n=members->memberList.size();
	thread threads[n];

	int i=0;


	Message msg=this->createMessage(TYPE_ASEQ, udp->processID, messageID, std::to_string(aseq));

	// send ASEQ message to all the members in the group
	for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
		string pid=(*it).first;
		Message expectedReply=createMessage(TYPE_ACK, pid, messageID,"");
		threads[i]=std::thread(&MessageStore::sendMessageTimeoutTo, this, pid, msg, expectedReply, "   sending ASEQ to "+pid+" ...");
		i++;
	}

	for(int j=0; j<i;j++){
		threads[j].join();
	}

	//mark deliverable
	holdbackQueue->updateAseq(convert_message_to_cpp(msg));
}


void MessageStore::sendMessageTimeoutTo(string processID, Message message, Message expectedReply, string printString){
	int trials=0;


	while(this->existMessage(expectedReply)==false && trials<MAX_TRAILS){
//		cout << printString + " Message: "+this->to_string(this->convert_message_to_cpp(message))<< endl;
		udp->send_msg(processID,message);
		trials++;
		std::this_thread::sleep_for (std::chrono::milliseconds(RESEND_INTERVAL));
	}


	if(this->existMessage(expectedReply)==false){
		members->reportDie(processID);
	}
}


void MessageStore::sendACK(string processID, Message m){
	udp->send_msg(processID,m);
}


void MessageStore::sendDATA(string content){
	int mid=maxMessageId;
	maxMessageId++;

	this->sendFourway(TYPE_DATA, mid, content, "   sending Chat DATA to ");
}

void MessageStore::sendFourway(ssize_t type, ssize_t messageID, string data, string printString){
	int n=members->memberList.size();
	thread threads[n];


	Message msg=this->createMessage(type, udp->processID, messageID, data);

	//put into its own queu
	if(this->existMessage(msg)==false){
		HoldBackQueueItem item;
		item.Seq = this->get_maxPSEQ();
		item.deliverable = false;
		item.m = this->convert_message_to_cpp(msg);
	    holdbackQueue->put(item);
	    this->putMessage(item.m);
	}



	int i=0;
	// send DATA message to all the members in the group
	for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
		string pid=(*it).first;
		Message expectedReply=createMessage(TYPE_PSEQ, pid, messageID,"");
		threads[i]=std::thread(&MessageStore::sendMessageTimeoutTo, this, pid, msg, expectedReply, printString+pid+" ...");
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

	if(maxPSEQ > messageStore->get_maxASEQ()){
		messageStore->set_maxASEQ(maxPSEQ);
	}

	messageStore->sendASEQ(messageID, maxPSEQ);
}


string MessageStore::to_string(Message2 m2){
	string type;
	switch(m2.type){
	case TYPE_ACK:
		type="ACK";
		break;
	case TYPE_ASEQ:
		type="ASEQ";
		break;
	case TYPE_DATA:
		type="DATA";
		break;
	case TYPE_DIE:
		type="DIE";
		break;
	case TYPE_HEARTBEAT:
		type="HEARTBEAT";
		break;
	case TYPE_JOIN:
		type="JOIN";
		break;
	case TYPE_LEAVE:
		type="LEAVE";
		break;
	case TYPE_NEW:
		type="NEW";
		break;
	case TYPE_PSEQ:
		type="PSEQ";
		break;
	default:
		type="?";
		break;
	}
	return m2.processId + " type: "+type+" mid: "+std::to_string(m2.messageId)+" content: "+m2.data;
}

