/*
 * MessageHistory.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "includeAll.h"
#include <unordered_map>
#include "message_format.h"
#include <string>

class Messages {
public:
	Messages();
	virtual ~Messages();
	sem_t lock;

	// max number that has sent
	ssize_t maxMessageId;

	//key has the following structure, linked with #
	//ssize_t type;
	//std::string processId;
	//ssize_t messageId;

	Message createMessage(ssize_t type, string processID, ssize_t messageID, string data);
	Message createMessage(Message* m);
	Message2 convert_message_to_cpp(Message m);
	string getIP(string ip_port);
	string getPort(string ip_port);

	ssize_t get_maxPSEQ();
	ssize_t get_maxASEQ();

	bool existMessage(ssize_t type, string processID, ssize_t messageID);
	bool existMessage(Message m);
	bool existMessage(Message2 m);

	string getMessageData(ssize_t type, string processID, ssize_t messageID);

	void putMessage(Message m);
	void putMessage(Message2 m);

	bool checkout(Message m);
	bool checkout(Message* m);

	ssize_t maxPSEQ;
	ssize_t maxASEQ;

	bool sendJOIN(string group_processId);
	void sendNEW();
	void sendASEQ(ssize_t messageID, int maxASEQ);
	void sendACK(string processID, Message m);
	void sendDATA(string content);

private:
	std::unordered_map<std::string,std::string> receivedMessages;
	string getKey(ssize_t type, string pid, ssize_t mid);
	void sendMessageTimeoutTo(string processID, Message message, Message expectedReply, string printString);
	void sendFourway(ssize_t type, ssize_t messageID, string data, string printString);
};

#endif /* MESSAGES_H_ */