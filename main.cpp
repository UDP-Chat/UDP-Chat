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
	t.join();
}

string getIP(string ip_port){
	std::string delimiter = ":";
	std::string ip = ip_port.substr(0, ip_port.find(delimiter));
	return ip;
}

string getPort(string ip_port){
	std::string ip = ip_port.substr(getIP(ip_port).length()+1);
	return ip;
}

void start_as_guest(string name, string group_address){
	std::thread t(&UDP::start_listen, udp, name);

	while(udp->started==false){
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}

	cout << group_address<<endl;
	cout << getIP(group_address)<<endl;
	cout << getPort(group_address)<<endl;

	Message msg;
	msg.type=TYPE_JOIN;
	msg.messageId=0;
	string pid=udp->processID;
	memcpy(msg.processId,pid.c_str(),pid.length()+1);
//	msg.processId=udp->ip+":"+to_string(udp->port);
//	msg.data=name;
	memcpy(msg.data,name.c_str(),name.length()+1);

	udp->udp_send_msg(getIP(group_address),getPort(group_address),msg);

	cout << "JOIN sent" << endl;

	t.join();
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

