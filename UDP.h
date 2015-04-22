/*
 * UDP.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef UDP_H_
#define UDP_H_


#include "includeAll.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>





class UDP {
public:
	string ip;
	int port;
	UDP();
	virtual ~UDP();
	ssize_t start_listen(string name);
	string getLocalIP();
	static const ssize_t BUF_SIZE=10240;
	void test(int i);
};

#endif /* UDP_H_ */