/*
 * message_format.h
 *
 *  Created on: Apr 14, 2015
 *      Author: cis455
 */

#ifndef MESSAGE_FORMAT_H_
#define MESSAGE_FORMAT_H_

#include <vector>

typedef std::pair<ssize_t, ssize_t> messagePair; // (process id, message id)
typedef struct {
	std::vector<messagePair>* receivedList;
	long time;
} ReceivedAndTime;
typedef struct {
	ssize_t processId;
	ssize_t messageId;
	ssize_t sequenceNum;
	ssize_t marker;
	char data[10240];
} Message;
typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	messagePair* candiMsg;  // process id and message id of candidate message
	ssize_t processedSequenceNumber; // processed sequence number
} PSEQ;

typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	messagePair* candiMsg;  // process id and message id of candidate message
	ssize_t agreedSequenceNumber; // agreed sequence number
} ASEQ;

typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	messagePair* aseqCandiMsg; // process id and message id of candidate aseq message
} ACK;

typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	char ip[32];    // ip address of member in group
	ssize_t port;      // port number of member in group
} JOIN;

typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	char list[10240];    // list of members in the group
} LIST;

typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	char ip[32];    // ip address of new member
	ssize_t port;      // port number of new member
} NEW;

typedef struct {
	ssize_t type;
	ssize_t processId; // process id
	ssize_t messageId; // message id
	char content[10240];    // content of message
	char ip[32];    // ip address of new message sender
	ssize_t port;      // port number of new message sender
} DATA;

struct tempList {
	char ip[32];  // ip to send to
	ssize_t port; // port to send to
	LIST* listMsg; // list message
};

struct tempNew {
	char ip[32];  // ip to send to
	ssize_t port; // port to send to
	NEW* newMsg; // new message
};

struct tempData {
	char ip[32];  // ip to send to
	ssize_t port; // port to send to
	DATA* dataMsg; // new message
};
struct tempPseq {
	char ip[32];  // ip to send to
	ssize_t port; // port to send to
	PSEQ* pseqMsg; // pseq message
};

struct tempAseq {
	char ip[32];  // ip to send to
	ssize_t port; // port to send to
	ASEQ* aseqMsg; // aseq message
};

struct tempAck {
	char ip[32];  // ip to send to
	ssize_t port; // port to send to
	ACK* ackMsg; // ack message
};
#endif /* MESSAGE_FORMAT_H_ */
