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

	void parseJOIN(Message msg);
	void parseList(Message msg);
	void parseNEWorDATA(Message msg);
	void parseASEQ(Message msg);
	void parseDATA(Message msg);

	void split(const std::string &s, char delim, std::vector<std::string> &elems);
	void addMember(string processid, string name);

	void reportDie(string pid);
};

#endif /* MEMBERS_H_ */
