/*
 * MessageHistory.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef MESSAGEHISTORY_H_
#define MESSAGEHISTORY_H_

#include "includeAll.h"
#include <unordered_map>
#include "message_format.h"
#include <string>

class MessageHistory {
public:
	MessageHistory();
	virtual ~MessageHistory();

	// max number that has sent
	ssize_t maxMessageId;

	//key has the following structure, linked with #
	//ssize_t type;
	//std::string processId;
	//ssize_t messageId;

	Message createMessage(ssize_t type, string processID, ssize_t messageID, string data);

	ssize_t get_maxPSEQ();
	ssize_t get_maxASEQ();

private:
	std::unordered_map<std::string,std::string> receivedMessage;

	ssize_t maxPSEQ;
	ssize_t maxASEQ;
};

#endif /* MESSAGEHISTORY_H_ */
