/*
 * HeartBeat.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "HeartBeat.h"
#include "global.h"


HeartBeat::HeartBeat() {
	// TODO Auto-generated constructor stub

}

HeartBeat::~HeartBeat() {
	// TODO Auto-generated destructor stub
}


void HeartBeat::parseMessage(Message message) {
	const long double sysTime = time(0);
	const long double sysTimeMS = sysTime*1000;
	members->memberList.find(message.processId)->second.time=sysTimeMS;
}

void HeartBeat::startMonitor(){

}
