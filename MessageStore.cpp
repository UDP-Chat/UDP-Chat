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

	sem_init(&lock_receivedMessages, 0, 1);
	sem_init(&lock_maxMessageId, 0, 1);
	sem_init(&lock_maxPSEQ, 0, 1);
	sem_init(&lock_maxASEQ, 0, 1);

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
	res.messageId=m.messageId;
	res.data=string(m.data);
	return res;
}

void MessageStore::removeMessage(Message m, bool lock){
	if(lock)sem_wait(&lock_receivedMessages);
	Message2 m2=this->convert_message_to_cpp(m);
	if(receivedMessages_locked.count(this->getKey(m2.type,m2.processId,m2.messageId))>0){
		this->receivedMessages_locked.erase(getKey(m2.type,m2.processId,m2.messageId));
	}
	if(lock)sem_post(&lock_receivedMessages);
}

bool MessageStore::existMessage(Message m, bool lock){
	return this->existMessage(this->convert_message_to_cpp(m), lock);
}

bool MessageStore::existMessage(Message2 m, bool lock){
	return this->existMessage(m.type, m.processId, m.messageId, lock);
}


bool MessageStore::existMessage(ssize_t type, string processID, ssize_t messageID, bool lock){
	if(lock)sem_wait(&lock_receivedMessages);
	bool res=receivedMessages_locked.count(this->getKey(type,processID,messageID))>0;
	if(lock)sem_post(&lock_receivedMessages);
	return res;
}

string MessageStore::getMessageData(ssize_t type, string processID, ssize_t messageID, bool lock){
	if(lock)sem_wait(&lock_receivedMessages);
	string res=receivedMessages_locked.find(this->getKey(type,processID,messageID))->second;
	if(lock)sem_post(&lock_receivedMessages);
	return res;
}

string MessageStore::updateMessageData(ssize_t type, string processID, ssize_t messageID, string new_data, bool lock){
	if(lock)sem_wait(&lock_receivedMessages);
	receivedMessages_locked.find(this->getKey(type,processID,messageID))->second=new_data;
	if(lock)sem_post(&lock_receivedMessages);
}


bool MessageStore::checkout(Message2 m){
	bool res;
	sem_wait(&lock_receivedMessages);
	if(this->existMessage(m, false)){
		res=true;
	}else{
		this->putMessage(m, false);
		res=false;
	}
	sem_post(&lock_receivedMessages);
	return res;
}
bool MessageStore::checkout(Message* m){
	return this->checkout(this->convert_message_to_cpp(this->createMessage(m)));
}

void MessageStore::putMessage(Message2 m2, bool lock){
	if(lock)sem_wait(&lock_receivedMessages);
	pair<string, string> p(this->getKey(m2.type,m2.processId,m2.messageId),m2.data);
	receivedMessages_locked.insert(p);
	if(lock)sem_post(&lock_receivedMessages);
}


string MessageStore::getKey(ssize_t type, string processId, ssize_t messageId){
	return std::to_string(type)+"#"+processId+"#"+std::to_string(messageId);
}









bool MessageStore::sendJOIN(string group_processId){
	Message msg=createMessage(TYPE_JOIN, udp->processID, MESSAGE_ID_JOIN,udp->name);

	Message expectedReturn=createMessage(TYPE_LIST, group_processId, MESSAGE_ID_LIST,"");

	int trials=0;

	while(this->existMessage(expectedReturn,true)==false && trials<MAX_TRAILS){
//		cout << "   sending JOIN..." << endl;
		udp->send_msg(group_processId,msg);
		trials++;
		std::this_thread::sleep_for (std::chrono::milliseconds(RESEND_INTERVAL));
	}

	if(this->existMessage(expectedReturn, true)){
		return true;
	}else{
		return false;
	}
}


void MessageStore::sendNEW(){
	this->sendFourway(TYPE_NEW, MESSAGE_ID_NEW, udp->name, "   sending NEW to ", true);
}


void MessageStore::sendASEQ(ssize_t messageID, int aseq, bool memberlock){

	if(memberlock)members->lock_members();
	int n=members->memberList_locked.size();
	if(memberlock)members->unlock_members();
	thread threads[n];

	int i=0;


	Message msg=this->createMessage(TYPE_ASEQ, udp->processID, messageID, std::to_string(aseq));

	// send ASEQ message to all the members in the group
	if(memberlock)members->lock_members();
	for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
		string pid=(*it).first;
		Message expectedReply=createMessage(TYPE_ACK, pid, messageID,"");
		threads[i]=std::thread(&MessageStore::sendMessageTimeoutTo, this, pid, msg, expectedReply, "   sending ASEQ to "+pid+" ...", true, memberlock);
		i++;
	}
	if(memberlock)members->unlock_members();

	for(int j=0; j<i;j++){
		threads[j].join();
	}

	//mark deliverable
	holdbackQueue->updateAseq(convert_message_to_cpp(msg), true);
}


void MessageStore::sendMessageTimeoutTo(string processID, Message message, Message expectedReply, string printString, bool reportNoResponse, bool lockMember){
	int trials=0;


	while(this->existMessage(expectedReply, true)==false && trials<MAX_TRAILS){
//		cout << printString + " Message: "+this->to_string(this->convert_message_to_cpp(message))<< endl;
		udp->send_msg(processID,message);
		trials++;
		std::this_thread::sleep_for (std::chrono::milliseconds(RESEND_INTERVAL));
	}


	if(this->existMessage(expectedReply, true)==false && reportNoResponse){
		members->reportNoResponse(processID, lockMember);
	}
}


void MessageStore::sendACK(string processID, Message m){
	udp->send_msg(processID,m);
}


void MessageStore::sendDATA(string content){
	int mid=maxMessageId;
	maxMessageId++;

	this->sendFourway(TYPE_DATA, mid, content, "   sending Chat DATA to ", true);
}

void MessageStore::sendFourway(ssize_t type, ssize_t messageID, string data, string printString, bool lock){

	if(lock)members->lock_members();
	int n=members->memberList_locked.size();
	if(lock)members->unlock_members();
	thread threads[n];



	Message msg=this->createMessage(type, udp->processID, messageID, data);


	sem_wait(&lock_receivedMessages);
	//put into its own queu
	if(this->existMessage(msg, false)==false){
		HoldBackQueueItem item;
		item.Seq = this->get_maxPSEQ();
		item.deliverable = false;
		item.m = this->convert_message_to_cpp(msg);
	    holdbackQueue->put(item, true);
	    this->putMessage(item.m, false);
	}
	sem_post(&lock_receivedMessages);


	int i=0;
	// send DATA message to all the members in the group

	if(lock)members->lock_members();
	for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
		string pid=(*it).first;
		Message expectedReply=createMessage(TYPE_PSEQ, pid, messageID,"");
		threads[i]=std::thread(&MessageStore::sendMessageTimeoutTo, this, pid, msg, expectedReply, printString+pid+" ...", true, lock);
		i++;
	}
	if(lock)members->unlock_members();

	for(int j=0; j<i;j++){
		threads[j].join();
	}


	//get max PSEQ from members, and prepare to send ASEQ
	int maxPSEQ=-1;
	if(lock)members->lock_members();
	for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
		string pid=(*it).first;
		sem_post(&lock_receivedMessages);
		if(messageStore->existMessage(TYPE_PSEQ, pid, messageID, false)){//if left ignore
			int pseq=atoi(messageStore->getMessageData(TYPE_PSEQ, pid, messageID, false).c_str());
			if(pseq>maxPSEQ)maxPSEQ=pseq;
		}
		sem_post(&lock_receivedMessages);
	}
	if(lock)members->unlock_members();

	if(maxPSEQ > messageStore->get_maxASEQ()){
		messageStore->set_maxASEQ(maxPSEQ);
	}

	messageStore->sendASEQ(messageID, maxPSEQ, true);
}

void MessageStore::sendLEAVE(string pid, bool lock){
	Message leave=messageStore->createMessage(TYPE_LEAVE, pid, MESSAGE_ID_LEAVE,"");
	if(lock)members->lock_members();
	for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
		string pid=(*it).first;
		udp->send_msg(pid, leave);
	}
	if(lock)members->unlock_members();
}


void MessageStore::sendASK_ASEQ(string processID, ssize_t messageID, bool lockMember){
	Message ask=createMessage(TYPE_ASK_ASEQ, udp->processID, messageID, processID);
	Message expectedReply=createMessage(TYPE_ASEQ, processID, messageID,"");

	if(lockMember)members->lock_members();
	for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
		string pid=(*it).first;
		messageStore->lock_received();
		this->sendMessageTimeoutTo(pid, ask, expectedReply, "   sending ASEQ to "+pid+" ...", false, lockMember);
	}
	if(lockMember)members->unlock_members();

	if(existMessage(expectedReply, true)==false){
		holdbackQueue->removeMessage(processID, messageID, true);
	}
}


void MessageStore::lock_received(){
	sem_wait(&lock_receivedMessages);
}
void MessageStore::unlock_received(){
	sem_post(&lock_receivedMessages);
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
		type=std::to_string(m2.type);
		break;
	}
	return m2.processId + " type: "+type+" mid: "+std::to_string(m2.messageId)+" content: "+m2.data;
}


string MessageStore::encrypt(string input){
	char *cstr = new char[input.length() + 1];
	size_t len = input.copy(cstr, input.length(),0);
	cstr[len] = '\0';

	for(int i=0;i<input.length();i++){
		if(cstr[i]+ROTATE>=ASCII){
			cstr[i]=(char)(cstr[i]+ROTATE-ASCII+START);
		}
		else
			cstr[i]=(char)(cstr[i]+ROTATE);
	}
	string res(cstr);
	delete [] cstr;
	return res;
}

string MessageStore::decrypt(string input){
	char *cstr = new char[input.length() + 1];
	size_t len = input.copy(cstr, input.length(),0);
	cstr[len] = '\0';

	for(int i=0;i<input.length();i++){
		if(cstr[i]-ROTATE<=START){
			cstr[i]=(char)(cstr[i]-ROTATE+ASCII-START);
		}
		else
			cstr[i]=(char)(cstr[i]-ROTATE);
	}


	string res(cstr);
	delete [] cstr;
	return res;
}
