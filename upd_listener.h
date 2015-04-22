/*
 * main_listener.h
 *
 *  Created on: Apr 19, 2015
 *      Author: yuexi
 */

#ifndef UPD_LISTENER_H_
#define UPD_LISTENER_H_

#include "includeAll.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <ifaddrs.h>
#include "buffer_parser.h"

using namespace std;

#define BUF_SIZE 10240

int start(string name);
string getLocalIP();

#endif /* UPD_LISTENER_H_ */



