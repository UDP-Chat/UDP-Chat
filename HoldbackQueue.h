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
	sem_t lock_queue;

	std::vector<HoldBackQueueItem> queue_locked;
	//std::unordered_map<std::string,HoldBackQueueItem> queueMap;

	void put(HoldBackQueueItem item, bool lock);
	void updateAseq(Message2 msg, bool lock);
	void removeMessage(string processID, ssize_t messageID, bool lock);
	void lock();
	void unlock();
private:
	static bool compareSeq(const HoldBackQueueItem&,const HoldBackQueueItem&);
	void findDeliverable(bool lock);
	void printQueue();
};

#endif /* HOLDBACKQUEUE_H_ */
