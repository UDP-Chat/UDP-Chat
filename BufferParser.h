/*
 * BufferParser.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef BUFFERPARSER_H_
#define BUFFERPARSER_H_

#include "includeAll.h"


class BufferParser {

#define TYPE_DATA 1
#define TYPE_PSEQ 2
#define TYPE_ASEQ 3
#define TYPE_ACK 4
#define TYPE_JOIN 5
#define TYPE_NEW 6
#define TYPE_LIST 7
#define TYPE_DIE 8
#define TYPE_LASTTIME 9
#define TYPE_LEAVE 10
#define TYPE_HEARTBEAT 11

public:
	BufferParser();
	virtual ~BufferParser();
	void parse_buffer(char* buffer);
};

#endif /* BUFFERPARSER_H_ */
