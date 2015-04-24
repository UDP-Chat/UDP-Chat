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
	sem_init(&lock_queue, 0, 1);

}

HoldbackQueue::~HoldbackQueue() {
}

void HoldbackQueue::lock(){
	sem_wait(&lock_queue);
}
void HoldbackQueue::unlock(){
	sem_post(&lock_queue);
}

void HoldbackQueue::put(HoldBackQueueItem item, bool lock){
	if(lock)this->lock();
	this->queue_locked.push_back(item);
	if(lock)this->unlock();
}

void HoldbackQueue::printQueue(){
//	for(int i=0;i<queue.size();i++){
//		cout << "     Deliverable: " << queue[i].deliverable << " Seq: " << queue[i].Seq << " Type: " << queue[i].m.type << " Data: " << queue[i].m.data <<endl;
//		cout << "           Processid: " << queue[i].m.processId << " Messageid: " << queue[i].m.messageId <<endl;
//	}
}

void HoldbackQueue::removeMessage(string processID, ssize_t messageID, bool lock){
	if(lock)this->lock();
	for(int i=0;i<this->queue_locked.size();i++){
		if(this->queue_locked[i].m.processId.compare(processID)==0 && holdbackQueue->queue_locked[i].m.messageId==messageID){
			this->queue_locked.erase(this->queue_locked.begin()+i);
			i--;
		}
	}
	if(lock)this->unlock();

}

void HoldbackQueue::findDeliverable(bool lock){
	if(lock)this->lock();
	// sort the queue to find any deliverable messages
	std::sort(queue_locked.begin(),queue_locked.end(),HoldbackQueue::compareSeq);

	//printQueue();
	// find deliverable messages
		for(int i=0;i<queue_locked.size();i++){
			if(queue_locked[i].deliverable){
				Message2 msg = queue_locked[i].m;
				if(queue_locked[i].m.type==TYPE_NEW){ // add a new member
					string name=msg.data;
					string pid=msg.processId;

					if(pid.compare(udp->processID)==0){

					}else{
						members->addMember(pid,name, true);
						cout << "NOTICE " << name << " joined on " << pid << endl;
					}

				}
				else{ // a chat message
					string name;
					if(msg.processId.compare(udp->processID)==0){
						name = udp->name;
					}else{
						name = members->getName(msg.processId, true);
					}

					string chat=msg.data;
					cout << name << ":: " << chat << endl;
				}
				// delete the already delivered message
				queue_locked.erase(queue_locked.begin()+i);
				i--;
			}else{
				break;
			}
		}

		if(lock)this->unlock();
}

void HoldbackQueue::updateAseq(Message2 msg, bool lock){
	printQueue();
	if(lock)this->lock();
	for(int i=0; i<queue_locked.size();i++){
		Message2 mq = queue_locked[i].m;
//		cout << "Received Aseq: processId "<<msg.processId<<" messageId: "<<msg.messageId<<endl;

		if(mq.processId.compare(msg.processId)==0 && mq.messageId==msg.messageId){
//			cout << "Find the deliverable msg in holdback queue" << endl;
			queue_locked[i].Seq = atoi(msg.data.c_str());
			queue_locked[i].deliverable=true;
			break;
		}
	}
	if(lock)this->unlock();
	findDeliverable(lock);
}
