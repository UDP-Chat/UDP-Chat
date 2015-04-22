/*
 * HeartBeat.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include "includeAll.h"
#include "global.h"

class HeartBeat {
public:
	HeartBeat();
	virtual ~HeartBeat();

	void parseMessage(void* buffer);
};

#endif /* HEARTBEAT_H_ */
