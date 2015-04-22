/*
 * Members.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "Members.h"
#include <sstream>

Members::Members() {
	// TODO Auto-generated constructor stub

}

Members::~Members() {
	// TODO Auto-generated destructor stub
}

void Members::parseJoin(Message buffer){

}

void Members::parseList(Message buf){
	std::vector<std::string> member;
	split(buffer.data,' ',member);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
