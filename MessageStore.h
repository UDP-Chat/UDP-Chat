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

	bool existMessage(ssize_t type, string processID, ssize_t messageID, bool lock);
	bool existMessage(Message m, bool lock);
	bool existMessage(Message2 m, bool lock);
	void removeMessage(Message m, bool lock);

	string getMessageData(ssize_t type, string processID, ssize_t messageID, bool lock);
	string updateMessageData(ssize_t type, string processID, ssize_t messageID, string new_data, bool lock);

	void putMessage(Message m, bool lock);
	void putMessage(Message2 m, bool lock);

	bool checkout(Message2 m);
	bool checkout(Message* m);



	bool sendJOIN(string group_processId);
	void sendNEW();
	void sendASEQ(ssize_t messageID, int maxASEQ, bool lock);
	void sendACK(string processID, Message m);
	void sendDATA(string content);
	string to_string(Message2 m2);
	void sendMessageTimeoutTo(string processID, Message message, Message expectedReply, string printString, bool reportNoResponse, bool lockMember);
	void sendLEAVE(string processID, bool lock);

	void sendASK_ASEQ(string processID, ssize_t messageID, bool lock);

	string encrypt(std::string);
	string decrypt(std::string);

	void lock_received();
	void unlock_received();

private:
	// max number that has sent
	ssize_t maxMessageId;
	ssize_t maxPSEQ;
	ssize_t maxASEQ;


	//key has the following structure, linked with #
	//ssize_t type;
	//std::string processId;
	//ssize_t messageId;
	std::unordered_map<std::string,std::string> receivedMessages_locked;
	string getKey(ssize_t type, string pid, ssize_t mid);
	void sendFourway(ssize_t type, ssize_t messageID, string data, string printString, bool lock);
};

#endif /* MESSAGESTORE_H_ */
