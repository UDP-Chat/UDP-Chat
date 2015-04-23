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
	sem_t lock;

	// key is the process id defined as "ip:port" string
	// value is the last contact time
	std::unordered_map<std::string,MemberInfo> memberList;

	void parseJOIN(Message2 msg);
	void parseList(Message2 msg);
	void parseNEWorDATA(Message2 msg);
	void parseASEQ(Message2 msg);
	void parseDATA(Message2 msg);

	void split(const std::string &s, char delim, std::vector<std::string> &elems);
	void addMember(string processid, string name);

	void reportDie(string pid);
	void sendLeave();

	void printMemberList();
};

#endif /* MEMBERS_H_ */
