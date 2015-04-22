/*
 * Members.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef MEMBERS_H_
#define MEMBERS_H_

#include "includeAll.h"
#include <vector>
#include <string>

using namespace std;

class Members {
public:
	Members();
	virtual ~Members();

	// each memberList element is the process id defined as "ip:port" string
	vector<std::string> memberList;

	void parseJoin(void* buffer);
};

#endif /* MEMBERS_H_ */
