/*
 * main_listener.h
 *
 *  Created on: Apr 19, 2015
 *      Author: yuexi
 */

#ifndef UPD_LISTENER_H_
#define UPD_LISTENER_H_



#include <iostream>
using namespace std;




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 10240



#include "buffer_parser.h"



int start(ssize_t port);


#endif /* UPD_LISTENER_H_ */



