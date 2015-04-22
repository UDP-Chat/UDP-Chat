/*
 * main.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: yuexi
 */

#include <iostream>
using namespace std;

#include <sstream>
#include "includeAll.h"

HeartBeat* heartBeat=new HeartBeat();
HoldbackQueue* holdbackQueue=new HoldbackQueue();
Members* members=new Members();
BufferParser* bufferParser=new BufferParser();
MessageHistory* messageHistory=new MessageHistory();
UDP* udp=new UDP();

void start_group(string name){

	udp->start_listen(name);
}

void start_as_guest(string name, string group_address){
	cout << name;
}

int main(int argc, char* argv[]){
	string name, group_address;

	stringstream ss;
	switch(argc){
	case 2:
		name=argv[1];

		start_group(name);

		break;
	case 3:
		name=argv[1];
		group_address=argv[2];

		start_as_guest(name, group_address);

		break;
	default:
		cout << "invalid number of argument";
	}
}

