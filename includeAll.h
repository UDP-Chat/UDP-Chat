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
#define TYPE_CHECKALIVE 12
#define TYPE_COMFIRMALIVE 13


#define MESSAGE_ID_JOIN -3
#define MESSAGE_ID_LIST -2
#define MESSAGE_ID_NEW -1
#define MESSAGE_ID_HEARTBEAT -5
#define MESSAGE_ID_CHECKALIVE -6
#define MESSAGE_ID_COMFIRMALIVE -7

#define MAX_TRAILS 13
#define RESEND_INTERVAL 300
#define HEARTBEAT_ALIVE_TIMEOUT 15

#include <thread>
#include <iostream>
#include <semaphore.h>

using namespace std;

#include "message_format.h"
#include "HeartBeat.h"
#include "HoldbackQueue.h"
#include "Members.h"
#include "MessageStore.h"
#include "UDP.h"
#include "BufferParser.h"


#endif /* INCLUDEALL_H_ */
