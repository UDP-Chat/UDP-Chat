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
Messages* messageStore=new Messages();
UDP* udp=new UDP();

void start_group(){
	std::thread t(&UDP::start_listen, udp, udp->name);

	while(udp->started==false){
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}

	cout << udp->name << " started a new chat, listening on "
				<< udp->processID << endl;
	cout << "Succeeded, current users:" << endl;
	cout << udp->name << " " << udp->processID << endl;
	cout << "Waiting for others to join..." << endl;
	t.join();

}


void start_as_guest(string group_address){
	std::thread t(&UDP::start_listen, udp, udp->name);

	while(udp->started==false){
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}

	cout << udp->name << " joing a new chat on " << group_address
			<<", listening on " << udp->processID << endl;
	//TODO print current user
	//cout << Succeeded, current

	if(messageStore->sendJOIN(group_address)){
		cout << "JOIN success" << endl;
	}else{
		cout << "JOIN fail" << endl;
		exit (EXIT_FAILURE);
	}

	t.join();
}



int main(int argc, char* argv[]){
	string name, group_address;

	stringstream ss;
	switch(argc){
	case 2:
		name=argv[1];
		udp->name = name;
		start_group();

		break;
	case 3:
		name=argv[1];
		group_address=argv[2];
		udp->name = name;
		start_as_guest(group_address);

		break;
	default:
		cout << "invalid number of argument";
	}
}

