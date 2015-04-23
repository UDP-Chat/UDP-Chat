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
	// TODO Auto-generated constructor stub

}

Members::~Members() {
	// TODO Auto-generated destructor stub
}

void Members::parseNew(Message buffer){
	// put into hold back queue
	HoldBackQueueItem item;
	int pseq = max(messageStore->Messages::get_maxPSEQ(),messageStore->Messages::get_maxASEQ())+1;
	item.Seq = pseq;
	item.deliverable = false;
	item.m = buffer;

}

void Members::parseJoin(Message buffer){
	// After received a Join message, should return a List message
	Message msg;

	string data,sendToIP,sendToPort;
	msg.type = TYPE_LIST;
	memcpy(msg.processId,udp->processID.c_str(),udp->processID.length()+1);
	msg.messageId=0;

	// add itself into the list
	data = udp->processID + "#" + udp->name;
	cout << data << endl;
	// add other members into the list
	for(auto it=memberList.begin();it!=memberList.end();++it){
		data = data + " " + (*it).first+"#"+(*it).second.name;
	}
	memcpy(msg.data,data.c_str(),data.length()+1);

	string s(buffer.processId);
	sendToIP = s.substr(0,s.find(":"));
	sendToPort = s.substr(s.find(":")+1);

	udp->udp_send_msg(sendToIP,sendToPort,msg);
}

void Members::parseList(Message msg){
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

	// send NEW message to all the members in the group
	for(auto it=memberList.begin();it!=memberList.end();++it){
		//TODO
		// This is a test for receive list message
		std::thread t(&Members::sendNew, members, (*it).first);
	}
}

void Members::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

}

void Members::sendNew(string processId){
	Message msg;
	msg.type=TYPE_NEW;
	msg.messageId=0;
	memcpy(msg.processId,udp->processID.c_str(),udp->processID.length()+1);
	memcpy(msg.data,udp->name.c_str(),udp->name.length()+1);
	string s(processId);
	string sendToIP = s.substr(0,s.find(":"));
	string sendToPort = s.substr(s.find(":")+1);
	udp->udp_send_msg(sendToIP,sendToPort,msg);
}

void Members::addMember(string processid, string name){
	MemberInfo mi;
	time_t t = time(0);
	mi.name = name;
	mi.time = (long)t;
	pair<std::string,MemberInfo> newPair(processid,mi);
	this->memberList.insert(newPair);
}
