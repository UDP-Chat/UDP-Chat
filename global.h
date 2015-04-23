/*
 * global.h
 *
 *  Created on: Apr 22, 2015
 *      Author: cis455
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "includeAll.h"

extern HeartBeat* heartBeat;
extern HoldbackQueue* holdbackQueue;
extern Members* members;
extern BufferParser* bufferParser;
extern UDP* udp;
extern MessageStore* messageStore;


#endif /* GLOBAL_H_ */
