/*
 * HoldbackQueue.h
 *
 *  Created on: Apr 22, 2015
 *      Author: yuexi
 */

#ifndef HOLDBACKQUEUE_H_
#define HOLDBACKQUEUE_H_

#include "includeAll.h"
#include "message_format.h"
#include <vector>

class HoldbackQueue {
public:
	HoldbackQueue();
	virtual ~HoldbackQueue();

	std::vector<HoldBackQueueItem> queue;
	//std::unordered_map<std::string,HoldBackQueueItem> queueMap;

	void put(HoldBackQueueItem item);
	void updateAseq(Message2 msg);
	void removeMessage(string processID, ssize_t messageID);
private:
	static bool compareSeq(const HoldBackQueueItem&,const HoldBackQueueItem&);
	void findDeliverable();
	void printQueue();
};

#endif /* HOLDBACKQUEUE_H_ */
