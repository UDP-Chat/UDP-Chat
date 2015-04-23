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

Members::Members() {
	sem_init(&lock, 0, 1);
}

Members::~Members() {
}

void Members::parseNEWorDATA(Message2 message){

	int pseq = max(messageStore->MessageStore::get_maxPSEQ(),messageStore->MessageStore::get_maxASEQ())+1;
	messageStore->set_maxPSEQ(pseq);
	if(messageStore->checkout(message)==false){
		// put into hold back queue
		HoldBackQueueItem item;
		item.Seq = pseq;
		item.deliverable = false;
		item.m = message;
		holdbackQueue->put(item);
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
	cout << data << endl;
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


void Members::reportDie(string pid){
	//TODO
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


