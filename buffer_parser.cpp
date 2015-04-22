/*
 * buffer_parser.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: yuexi
 */


#include "buffer_parser.h"

void* parse_buffer(void* buffer){
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
//		members.parseJoin(buffer);
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
//		heartBeat.parseMessage(buffer);
		break;
	default:
		break;
	}

	return (void*)type;
}
