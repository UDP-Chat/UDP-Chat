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
#include <string.h>

class MessageHistory {
public:
	MessageHistory();
	virtual ~MessageHistory();

	// max number that has sent
	ssize_t maxMessageId;
	// received ack, this member's message id as key, ACK sender's process id as value
	std::unordered_map<ssize_t, std::string> receivedAck;
	// received pseq, this member's message id as key, received PSEQ as value
	std::unordered_map<ssize_t, PSEQ> receivedPseq;
};

#endif /* MESSAGEHISTORY_H_ */
