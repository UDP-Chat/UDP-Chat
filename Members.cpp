/*
 * Members.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "Members.h"
#include "global.h"
#include <sstream>
#include <ctime>
#include <unordered_map>
#include <semaphore.h>
#include <vector>

Members::Members() {
	sem_init(&lock_memberList, 0, 1);
}

Members::~Members() {
}

void Members::parseNEWorDATA(Message2 message){
	int pseq;
	if(messageStore->checkout(message)==false){
		pseq = max(messageStore->MessageStore::get_maxPSEQ(),messageStore->MessageStore::get_maxASEQ())+1;
		messageStore->set_maxPSEQ(pseq);
		// put into hold back queue
		HoldBackQueueItem item;
		item.Seq = pseq;
		item.deliverable = false;
		item.m = message;

		//save pseq into DATA message data
//		messageStore->updateMessageData(message.type, message.processId, message.messageId, to_string(pseq));
		Message2 update=message;
		update.data=to_string(pseq);
		messageStore->putMessage(update);

		holdbackQueue->put(item);
	}else{
		pseq=atoi(messageStore->getMessageData(message.type, message.processId, message.messageId).c_str());
	}

	// send PSEQ
	Message msg=messageStore->createMessage(TYPE_PSEQ, udp->processID, message.messageId, to_string(pseq));
	udp->send_msg(message.processId,msg);
}

void Members::parseJOIN(Message2 message){
	// After received a Join message, should return a List message

	string data;
	// add itself into the list
	data = udp->processID + "#" + udp->name;

	// add other members into the list
	for(auto it=memberList.begin();it!=memberList.end();++it){
		data = data + " " + (*it).first+"#"+(*it).second.name;
	}

	Message msg=messageStore->createMessage(TYPE_LIST, udp->processID, MESSAGE_ID_LIST, data);


	udp->send_msg(message.processId,msg);
}

void Members::parseList(Message2 msg){
	std::vector<std::string> member;
	string dataString(msg.data);
	split(dataString,' ',member);
	time_t t = time(0);

	// copy the member list into its own memberList unordered_map
	for(std::vector<std::string>::iterator it = member.begin();it!=member.end();++it){
		MemberInfo mi;
		mi.name=(*it).substr((*it).find('#')+1);
		mi.time=(long)t;
		string ipport=(*it).substr(0,(*it).find('#'));
		pair<std::string,MemberInfo> ele(ipport,mi);
		memberList.insert(ele);
	}

	messageStore->sendNEW();

	cout << "JOIN success! Start chat!"<<endl;


}

void Members::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

}


void Members::addMember(string processid, string name){
	MemberInfo mi;
	time_t t = time(0);
	mi.name = name;
	mi.time = (long)t;
	pair<std::string,MemberInfo> newPair(processid,mi);
	this->memberList.insert(newPair);
}

void Members::printMemberList(){
	for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
					cout << (*it).second.name << " " << (*it).first<<endl;
			}
}

void Members::reportNoResponse(string pid){
//	cout<<"REPORTING LEAVE "+pid<<endl;

	//send report to others
	std::thread t(&MessageStore::sendLEAVE, messageStore, pid);
	t.detach();

	//send report to self
	Message leave=messageStore->createMessage(TYPE_LEAVE, pid, MESSAGE_ID_LEAVE,"");
	udp->send_msg(udp->processID, leave);
//	std::thread t2(&BufferParser::parse_buffer, bufferParser, (Message*)&leave);
//	t2.detach();
}

string Members::getName(string pid){
	if(memberList.count(pid)>0){
		return memberList.find(pid)->second.name;
	}else{
		return pid;
	}
}

void Members::parseASEQ(Message2 msg){
	if(messageStore->checkout(msg)==false){
		// update aseq
		ssize_t aseq = atoi(msg.data.c_str());
		if(aseq > messageStore->get_maxASEQ()){
			messageStore->set_maxASEQ(aseq);
		}
		// deliever
		holdbackQueue->updateAseq(msg);
	}

	Message ack=messageStore->createMessage(TYPE_ACK, udp->processID, msg.messageId, "");
	messageStore->sendACK(msg.processId, ack);
}


void Members::parseLEAVE(Message2 msg){

	if(members->memberList.count(msg.processId)>0){
		cout << "NOTICE "+this->getName(msg.processId)+" on "+msg.processId+" left the chat group." << endl;

		//		members->removeMember(processID);
		members->memberList.erase(members->memberList.find(msg.processId));
	}

	std::vector<Message2> list;
	for(int i=0; i< holdbackQueue->queue.size();i++ ){
		Message2 pending =holdbackQueue->queue[i].m;
		list.push_back(pending);
	}

	for(int i=0;i<list.size();i++){
		Message2 m=list[i];
		std::thread t(&MessageStore::sendASK_ASEQ, messageStore, m.processId, m.messageId);
		t.detach();
	}

}
