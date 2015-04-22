/*
 * Members.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "Members.h"
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

}

void Members::parseJoin(Message buffer){
	// After received a Join message, should return a List message
	Message msg;
	string processid(udp->processID);
	string data,sendToIP,sendToPort;
	msg.type = TYPE_LIST;
	memcpy(msg.processId,processid.c_str(),processid.length()+1);

	// add itself into the list
	data = processid + "#" + udp->name;
	// add other members into the list
	for(auto it=memberList.begin();it!=memberList.end();++it){
		data = data + " " + (*it).first+"#"+(*it).second.name;
	}
	memcpy(msg.data,data.c_str(),sizeof(data)+1);
	string s(buffer.processId);
	sendToIP = s.substr(0,s.find(":"));
	sendToPort = s.substr(s.find(":")+1);

	udp->udp_send_msg(sendToIP,sendToPort,msg);
}

void Members::parseList(Message msg){
	std::vector<std::string> member;
	split(msg.data,' ',member);
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
		cout<<(*it).first<<endl;
		cout<<(*it).second.name<<endl;
		cout<<(*it).second.time<<endl;
		cout<<"==========================";
	}
}

void Members::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

}

void Members::sendNew(){

}
