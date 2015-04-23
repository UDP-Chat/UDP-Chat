/*
 * HoldbackQueue.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#include "HoldbackQueue.h"
#include <algorithm>
#include "global.h"

bool HoldbackQueue::compareSeq(const HoldBackQueueItem& item1,const HoldBackQueueItem& item2){
	return item1.Seq < item2.Seq;
}

HoldbackQueue::HoldbackQueue() {
	// TODO Auto-generated constructor stub

}

HoldbackQueue::~HoldbackQueue() {
	// TODO Auto-generated destructor stub
}

void HoldbackQueue::put(HoldBackQueueItem item){
	this->queue.push_back(item);
}

void HoldbackQueue::printQueue(){
//	for(int i=0;i<queue.size();i++){
//		cout << "     Deliverable: " << queue[i].deliverable << " Seq: " << queue[i].Seq << " Type: " << queue[i].m.type << " Data: " << queue[i].m.data <<endl;
//		cout << "           Processid: " << queue[i].m.processId << " Messageid: " << queue[i].m.messageId <<endl;
//	}
}

void HoldbackQueue::removeMessage(string processID, ssize_t messageID){
	for(int i=0;i<holdbackQueue->queue.size();i++){
		if(holdbackQueue->queue[i].m.processId.compare(processID)==0 && holdbackQueue->queue[i].m.messageId==messageID){
			holdbackQueue->queue.erase(holdbackQueue->queue.begin()+i);
			i--;
		}
	}

}

void HoldbackQueue::findDeliverable(){
	// sort the queue to find any deliverable messages
	std::sort(queue.begin(),queue.end(),HoldbackQueue::compareSeq);

	//printQueue();
	// find deliverable messages
		for(int i=0;i<queue.size();i++){
			if(queue[i].deliverable){
				Message2 msg = queue[i].m;
				if(queue[i].m.type==TYPE_NEW){ // add a new member
					string name=msg.data;
					string pid=msg.processId;

					if(pid.compare(udp->processID)==0){

					}else{
						members->addMember(pid,name);
						cout << "NOTICE " << name << " joined on " << pid << endl;
					}

				}
				else{ // a chat message
					string name;
					if(msg.processId.compare(udp->processID)==0){
						name = udp->name;
					}else{
						name = members->getName(msg.processId);
					}

					string chat=msg.data;
					cout << name << ":: " << chat << endl;
				}
				// delete the already delivered message
				queue.erase(queue.begin()+i);
				i--;
			}else{
				break;
			}
		}
}

void HoldbackQueue::updateAseq(Message2 msg){
	printQueue();
	for(int i=0; i<queue.size();i++){
		Message2 mq = queue[i].m;
//		cout << "Received Aseq: processId "<<msg.processId<<" messageId: "<<msg.messageId<<endl;

		if(mq.processId.compare(msg.processId)==0 && mq.messageId==msg.messageId){
//			cout << "Find the deliverable msg in holdback queue" << endl;
			queue[i].Seq = atoi(msg.data.c_str());
			queue[i].deliverable=true;
			break;
		}
	}
	findDeliverable();
}
