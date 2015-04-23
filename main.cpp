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
	std::thread t(&UDP::start_listen, udp, name);

	while(udp->started==false){
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}

	cout << name << " started a new chat, listening on "
				<< udp->processID << endl;
	cout << "Succeeded, current users:" << endl;
	cout << name << " " << udp->processID << endl;
	cout << "Waiting for others to join..." << endl;
	t.join();

}


void start_as_guest(string name, string group_address){
	std::thread t(&UDP::start_listen, udp, name);

	while(udp->started==false){
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}

	cout << udp->name << " joing a new chat on " << group_address
			<<", listening on " << udp->processID << endl;
	//TODO print current user
	//cout << Succeeded, current

	Message msg=messageHistory->createMessage(TYPE_JOIN,udp->processID,0,name);

	udp->udp_send_msg(messageHistory->getIP(group_address),messageHistory->getPort(group_address),msg);

	cout << "JOIN sent" << endl;

	t.join();
}



int main(int argc, char* argv[]){
	string name, group_address;

	stringstream ss;
	switch(argc){
	case 2:
		name=argv[1];
		udp->name = name;
		start_group(name);

		break;
	case 3:
		name=argv[1];
		group_address=argv[2];
		udp->name = name;
		start_as_guest(name, group_address);

		break;
	default:
		cout << "invalid number of argument";
	}
}

