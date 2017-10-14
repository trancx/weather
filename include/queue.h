/*
 * queue.h
 *
 *  Created on: Sep 25, 2017
 *      Author: trance
 */

#ifndef INCLUDE_QUEUE_H_
#define INCLUDE_QUEUE_H_

#include <list.h>

	/* queue.c */
extern void * thread_do_query(void * );
extern void * thread_do_notify(void *);
extern void notify(struct query *);
extern void send_attach(struct query *);
extern void receive_attach(struct query *);


#endif /* INCLUDE_QUEUE_H_ */
