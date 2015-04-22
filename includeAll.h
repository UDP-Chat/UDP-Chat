/*
 * includeAall.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef INCLUDEALL_H_
#define INCLUDEALL_H_

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

#include <thread>
#include <iostream>
#include <semaphore.h>

using namespace std;

#include "HeartBeat.h"
#include "HoldbackQueue.h"
#include "Members.h"
#include "MessageHistory.h"
#include "UDP.h"
#include "message_format.h"
#include "BufferParser.h"


#endif /* INCLUDEALL_H_ */
