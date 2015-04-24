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
#define TYPE_LEAVE 8
#define TYPE_HEARTBEAT 9
#define TYPE_CHECKALIVE 10
#define TYPE_COMFIRMALIVE 11
#define TYPE_ASK_ASEQ 12


#define MESSAGE_ID_JOIN -1
#define MESSAGE_ID_LIST -2
#define MESSAGE_ID_NEW -3
#define MESSAGE_ID_HEARTBEAT -4
#define MESSAGE_ID_CHECKALIVE -5
#define MESSAGE_ID_COMFIRMALIVE -6
#define MESSAGE_ID_LEAVE -7

#define MAX_TRAILS 13
#define RESEND_INTERVAL 100000
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
