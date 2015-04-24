/*
 * HeartBeat.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "HeartBeat.h"
#include "global.h"


HeartBeat::HeartBeat() {
}

HeartBeat::~HeartBeat() {
}


void HeartBeat::parseMessage(Message2 message) {
	time_t t = time(0);

	if(members->memberList.count(message.processId)>0){
		members->memberList.find(message.processId)->second.time=(long)t;
	}
	Message reply;
	switch(message.type){
	case TYPE_CHECKALIVE:
		reply=messageStore->createMessage(TYPE_COMFIRMALIVE, udp->processID, MESSAGE_ID_COMFIRMALIVE, "");
		udp->send_msg(message.processId,reply);
		break;
	case TYPE_COMFIRMALIVE:
		messageStore->checkout(message);
		break;
	default:
		break;
	}
}

void HeartBeat::startMonitor(){
	std::thread send(&HeartBeat::sendHeartBeat, this);
	send.detach();

	std::thread check(&HeartBeat::checkAlive, this);
	check.detach();
}

void HeartBeat::checkAlive(){
	while(true){
		std::this_thread::sleep_for (std::chrono::seconds(HEARTBEAT_ALIVE_TIMEOUT));
		long now = (long)time(0);

//		cout << "beat: members " << members->memberList.size()<<endl;

		for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
			long lasttime=(*it).second.time;

			if(now-lasttime>HEARTBEAT_ALIVE_TIMEOUT){
				std::thread confirm(&HeartBeat::confirmBeat, this, (*it).first);
				confirm.detach();
			}


		}
	}
}


void HeartBeat::confirmBeat(string pid){
	Message message=messageStore->createMessage(TYPE_CHECKALIVE , pid, MESSAGE_ID_CHECKALIVE, "");
	Message expectedReply=messageStore->createMessage(TYPE_COMFIRMALIVE, pid, MESSAGE_ID_COMFIRMALIVE, "");
	if(messageStore->existMessage(expectedReply)){
		messageStore->removeMessage(expectedReply);
	}
	messageStore->sendMessageTimeoutTo(pid, message, expectedReply, "   confirming alive with "+pid, true);

}

void HeartBeat::sendHeartBeat(){
	while(true){
		std::this_thread::sleep_for (std::chrono::seconds(5));
		for(auto it=members->memberList.begin();it!=members->memberList.end();++it){
			string pid=(*it).first;
			Message beat=messageStore->createMessage(TYPE_HEARTBEAT, udp->processID, MESSAGE_ID_HEARTBEAT, "");
			udp->send_msg(pid, beat);
		}
	}
}
