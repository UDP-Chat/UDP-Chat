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
MessageStore* messageStore=new MessageStore();
UDP* udp=new UDP();

void inputLoop(){
	string input;
	while(true){
		cin >> input;
		if(cin.eof()){
			cout << "Quitting chat..." << endl;
			break;
		}else{
			cout << "   sending chat..."<<endl;
			messageStore->sendDATA(input);

//			if(input.find("printmembers")!=string::npos){
//				members->printMemberList();
//			}
		}
	}
}

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

	inputLoop();

	t.join();

}


void start_as_guest(string group_address){
	std::thread t(&UDP::start_listen, udp, udp->name);

	while(udp->started==false){
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}

	cout << udp->name << " joing a new chat on " << group_address
			<<", listening on " << udp->processID << endl;

	// print out current user list
	if(messageStore->sendJOIN(group_address)){
		cout << "Got list. current users:" << endl;
		for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
				cout << (*it).second.name << " " << (*it).first<<endl;
		}
	}else{
		cout << "Sorry, no chat is active on " << group_address<< ", try again later. Bye." << endl;
		exit (EXIT_FAILURE);
	}


	inputLoop();

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

