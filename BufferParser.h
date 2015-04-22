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



public:
	BufferParser();
	virtual ~BufferParser();
	void parse_buffer(Message buffer);
};

#endif /* BUFFERPARSER_H_ */
