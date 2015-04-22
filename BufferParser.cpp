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
	memcpy(m.processId,message->processId,strlen(message->processId));
	memcpy(m.data,message->data,strlen(message->data));
	m.messageId=message->messageId;
	m.type=message->type;

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
		cout<<"hi";
		members->parseJoin(m);
		cout<<"e";
		break;
	case TYPE_NEW:
		members->parseNew(m);
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
