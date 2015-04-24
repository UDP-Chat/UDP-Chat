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


void HeartBeat::parseMessage(Message2 message, bool lock) {
	time_t t = time(0);

	if(lock)members->lock_members();
	if(members->memberList_locked.count(message.processId)>0){
		members->memberList_locked.find(message.processId)->second.time=(long)t;
	}
	if(lock)members->unlock_members();

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
	std::thread send(&HeartBeat::sendHeartBeat, this, true);
	send.detach();

	std::thread check(&HeartBeat::checkAlive, this, true);
	check.detach();
}

void HeartBeat::checkAlive(bool lock){
	while(true){
		std::this_thread::sleep_for (std::chrono::seconds(HEARTBEAT_ALIVE_TIMEOUT));
		long now = (long)time(0);

//		cout << "beat: members " << members->memberList.size()<<endl;

		if(lock)members->lock_members();
		for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
			long lasttime=(*it).second.time;

			if(now-lasttime>HEARTBEAT_ALIVE_TIMEOUT){
				std::thread confirm(&HeartBeat::confirmBeat, this, (*it).first, true);
				confirm.detach();
			}
		}
		if(lock)members->unlock_members();
	}
}


void HeartBeat::confirmBeat(string pid, bool lockMember){
	Message message=messageStore->createMessage(TYPE_CHECKALIVE , pid, MESSAGE_ID_CHECKALIVE, "");
	Message expectedReply=messageStore->createMessage(TYPE_COMFIRMALIVE, pid, MESSAGE_ID_COMFIRMALIVE, "");

	if(lockMember)members->lock_members();
	if(messageStore->existMessage(expectedReply,false)){
		messageStore->removeMessage(expectedReply,false);
	}
	if(lockMember)members->unlock_members();

	messageStore->sendMessageTimeoutTo(pid, message, expectedReply, "   confirming alive with "+pid, true, lockMember);

}

void HeartBeat::sendHeartBeat(bool lock){
	while(true){
		std::this_thread::sleep_for (std::chrono::seconds(1));
		if(lock)members->lock_members();
		for(auto it=members->memberList_locked.begin();it!=members->memberList_locked.end();++it){
			string pid=(*it).first;
			Message beat=messageStore->createMessage(TYPE_HEARTBEAT, udp->processID, MESSAGE_ID_HEARTBEAT, "");
			udp->send_msg(pid, beat);
		}
		if(lock)members->unlock_members();
	}
}
