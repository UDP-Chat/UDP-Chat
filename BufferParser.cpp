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
	Message m=messageStore->createMessage(message);
	heartBeat->parseMessage(m);

	switch(m.type){
	case TYPE_DATA://finished
		members->parseNEWorDATA(m);
		break;
	case TYPE_PSEQ://finished.
		messageStore->checkout(m);
		break;
	case TYPE_ASEQ:
		members->parseASEQ(m);
		break;
	case TYPE_ACK://finished.
		messageStore->checkout(m);
		break;
	case TYPE_JOIN://finished.
		members->parseJOIN(m);
		break;
	case TYPE_NEW://finished.
		members->parseNEWorDATA(m);
		break;
	case TYPE_LIST://finished.
		if(messageStore->checkout(m))return;
		members->parseList(m);
		break;
	case TYPE_DIE:
		break;
	case TYPE_LEAVE:
		break;
	case TYPE_HEARTBEAT://finished.
		break;
	default:
		break;
	}
}
