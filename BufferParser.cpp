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


void BufferParser::parse_buffer(char* buffer){
	ssize_t* type = (ssize_t*) buffer;
	*type = ntohl(*type);

	switch(*type){
	case TYPE_DATA:
		break;
	case TYPE_PSEQ:
		break;
	case TYPE_ASEQ:
		break;
	case TYPE_ACK:
		break;
	case TYPE_JOIN:
		members->parseJoin(buffer);
		break;
	case TYPE_NEW:
		break;
	case TYPE_LIST:
		break;
	case TYPE_DIE:
		break;
	case TYPE_LEAVE:
		break;
	case TYPE_HEARTBEAT:
		heartBeat->parseMessage(buffer);
		break;
	default:
		break;
	}
}