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

	Message2 m2=messageStore->convert_message_to_cpp(m);

	cout << "   received message from "+to_string(m2.messageId) + " type: "+to_string(m2.type)+" mid: "+to_string(m2.messageId)+" content: "+m2.data<< endl;

	switch(m.type){
	case TYPE_DATA://finished
		members->parseNEWorDATA(m2);
		break;
	case TYPE_PSEQ://finished.
		messageStore->checkout(m2);
		break;
	case TYPE_ASEQ:
		members->parseASEQ(m2);
		break;
	case TYPE_ACK://finished.
		messageStore->checkout(m2);
		break;
	case TYPE_JOIN://finished.
		members->parseJOIN(m2);
		break;
	case TYPE_NEW://finished.
		members->parseNEWorDATA(m2);
		break;
	case TYPE_LIST://finished.
		if(messageStore->checkout(m2))return;
		members->parseList(m2);
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
