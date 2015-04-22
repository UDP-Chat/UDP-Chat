/*
 * BufferParser.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "BufferParser.h"
#include "global.h"

BufferParser::BufferParser() {
	// TODO Auto-generated constructor stub

}

BufferParser::~BufferParser() {
	// TODO Auto-generated destructor stub
}



void BufferParser::parse_buffer(Message* message){
	Message m;
	m.messageId=message->messageId;
	m.processId=message->processId;
	m.type=message->type;
	m.data=message->data;

	switch(message->type){
	case TYPE_DATA:
		break;
	case TYPE_PSEQ:
		break;
	case TYPE_ASEQ:
		break;
	case TYPE_ACK:
		break;
	case TYPE_JOIN:
		members->parseJoin(m);
		break;
	case TYPE_NEW:
		break;
	case TYPE_LIST:
		members->parseList(m);
		break;
	case TYPE_DIE:
		break;
	case TYPE_LEAVE:
		break;
	case TYPE_HEARTBEAT:

		heartBeat->parseMessage(m);
		break;
	default:
		break;
	}
}
