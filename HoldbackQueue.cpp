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

void HoldbackQueue::findDeliverable(){
	// sort the queue to find any deliverable messages
	std::sort(queue.begin(),queue.end(),HoldbackQueue::compareSeq);

	// find deliverable messages
		for(int i=0;i<queue.size();i++){
			if(queue[i].deliverable){
				Message2 msg = queue[i].m;
				string name=msg.data;
				if(queue[i].m.type==TYPE_NEW){ // add a new member
					string pid(msg.processId);

					members->addMember(pid,name);
					cout << "NOTICE " << name << " joined on " << pid << endl;
				}
				else{ // a chat message
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
	for(int i;i<queue.size();i++){
		Message2 mq = queue[i].m;

		if(mq.processId.compare(msg.processId)==0 && mq.messageId==msg.messageId){
			queue[i].Seq = atoi(msg.data.c_str());
			queue[i].deliverable=true;
			break;
		}
	}
	findDeliverable();
}
