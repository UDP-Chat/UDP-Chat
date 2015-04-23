/*
 * Members.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef MEMBERS_H_
#define MEMBERS_H_

#include "includeAll.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "message_format.h"

using namespace std;

class Members {
public:
	Members();
	virtual ~Members();

	// key is the process id defined as "ip:port" string
	// value is the last contact time
	std::unordered_map<std::string,MemberInfo> memberList;

	void parseJoin(Message msg);
	void parseList(Message msg);
	void parseNew(Message msg);

	void sendNew(string processid);
	void split(const std::string &s, char delim, std::vector<std::string> &elems);
	void addMember(string processid, string name);
};

#endif /* MEMBERS_H_ */
