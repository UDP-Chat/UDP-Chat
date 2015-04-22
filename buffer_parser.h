/*
 * buffer_parser.h
 *
 *  Created on: Apr 20, 2015
 *      Author: yuexi
 */

#ifndef BUFFER_PARSER_H_
#define BUFFER_PARSER_H_

#include "includeAll.h"
#include <iostream>
using namespace std;

#include <arpa/inet.h>




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

void* parse_buffer(void* buffer);


#endif /* BUFFER_PARSER_H_ */
