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

void Members::parseJoin(Message buffer){

}

void Members::parseList(Message msg){
	std::vector<std::string> member;
	split(msg.data,' ',member);
	time_t t = time(0);

	// copy the member list into its own memberList unordered_map
	for(std::vector<std::string>::iterator it = member.begin();it!=member.end();++it){
		pair<std::string,long> ele((*it),(long)t);
		memberList.insert(ele);
	}

	//
}

void Members::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

}
