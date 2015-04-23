/*
 * HeartBeat.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include "includeAll.h"
#include "message_format.h"

class HeartBeat {
public:
	HeartBeat();
	virtual ~HeartBeat();

	void parseMessage(Message2 message);

	void startMonitor();

private:
	void checkAlive();
	void sendHeartBeat();
	void confirmBeat(string pid);
};

#endif /* HEARTBEAT_H_ */
