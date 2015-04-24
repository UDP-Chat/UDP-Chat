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
	sem_t lock_memberList;

	// key is the process id defined as "ip:port" string
	// value is the last contact time
	std::unordered_map<std::string,MemberInfo> memberList_locked;

	void parseJOIN(Message2 msg);
	void parseList(Message2 msg);
	void parseNEWorDATA(Message2 msg);
	void parseASEQ(Message2 msg);
	void parseDATA(Message2 msg);
	void parseLEAVE(Message2 msg, bool lockmember, bool lockqueue);

	void split(const std::string &s, char delim, std::vector<std::string> &elems);
	void addMember(string processid, string name, bool lock);


	void reportNoResponse(string pid, bool lock);
	string getName(string pid, bool lock);

	void lock_members();
	void unlock_members();

	void printMemberList(bool lock);

};

#endif /* MEMBERS_H_ */
