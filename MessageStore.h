/*
 * MessageHistory.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef MESSAGESTORE_H_
#define MESSAGESTORE_H_

#include "includeAll.h"
#include <unordered_map>
#include "message_format.h"
#include <string>

class MessageStore {
public:
	MessageStore();
	virtual ~MessageStore();
	sem_t lock_receivedMessages;
	sem_t lock_maxMessageId;
	sem_t lock_maxPSEQ;
	sem_t lock_maxASEQ;



	Message createMessage(ssize_t type, string processID, ssize_t messageID, string data);
	Message createMessage(Message* m);
	Message2 convert_message_to_cpp(Message m);
	string getIP(string ip_port);
	string getPort(string ip_port);

	ssize_t get_maxPSEQ();
	ssize_t get_maxASEQ();
	void set_maxPSEQ(ssize_t);
	void set_maxASEQ(ssize_t);

	bool existMessage(ssize_t type, string processID, ssize_t messageID);
	bool existMessage(Message m);
	bool existMessage(Message2 m);
	void removeMessage(Message m);

	string getMessageData(ssize_t type, string processID, ssize_t messageID);
	string updateMessageData(ssize_t type, string processID, ssize_t messageID, string new_data);

	void putMessage(Message m);
	void putMessage(Message2 m);

	bool checkout(Message2 m);
	bool checkout(Message* m);



	bool sendJOIN(string group_processId);
	void sendNEW();
	void sendASEQ(ssize_t messageID, int maxASEQ);
	void sendACK(string processID, Message m);
	void sendDATA(string content);
	string to_string(Message2 m2);
	void sendMessageTimeoutTo(string processID, Message message, Message expectedReply, string printString, bool reportNoResponse);
	void sendLEAVE(string processID);

	void sendASK_ASEQ(string processID, ssize_t messageID);

	string encrypt(std::string);
	string decrypt(std::string);

private:
	// max number that has sent
	ssize_t maxMessageId;
	ssize_t maxPSEQ;
	ssize_t maxASEQ;


	//key has the following structure, linked with #
	//ssize_t type;
	//std::string processId;
	//ssize_t messageId;
	std::unordered_map<std::string,std::string> receivedMessages;
	string getKey(ssize_t type, string pid, ssize_t mid);
	void sendFourway(ssize_t type, ssize_t messageID, string data, string printString);
};

#endif /* MESSAGESTORE_H_ */
