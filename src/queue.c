/*
 * queue.c
 *
 *	we attach request to list, to make an inqury
 *	when invoking inqury(), then the query will
 *	make a real query!
 *
 *	and attach result to the list, when recieved
 *	result, and call notify() to laungh an event
 *
 *	mutex_lock in case of race condition!
 *
 *
 *
 *  Created on: Sep 24, 2017
 *      Author: trance
 */
#include "list.h"
#include <pthread.h>
#include "kernel.h"
#include "query.h"

LIST_HEAD(query_q);
LIST_HEAD(result_q);


/* 	query mutex lock, uh..  */
pthread_mutex_t q_mutex;
pthread_mutexattr_t q_attr;
	/*	condition */
pthread_cond_t q_cond = PTHREAD_COND_INITIALIZER;

/* 	result mutex lock, uh..  */
pthread_mutex_t r_mutex;
pthread_mutexattr_t r_attr;
	/*	condition */
pthread_cond_t r_cond = PTHREAD_COND_INITIALIZER;


/*	if the list isn't empty, then we lock the queue*/
void * thread_do_query( void * arg ) {
	struct query * q;
	while( 1 ) {
		pthread_mutex_lock(&q_mutex);
		while( list_empty(&query_q)) {
			pr_debug("Fun: %s, do_query", __FUNCTION__);
			pthread_cond_wait(&q_cond, &q_mutex);
		}
		/*now the list if not empty, and we get the lock*/
		q = list_first_entry(&query_q, struct query, q_node);
		list_del(&q->q_node);
		build_inquiry(q); /* now to build the request*/
		pthread_mutex_unlock(&q_mutex); /* unlock */
	}


	return NULL;
}

void * thread_do_notify( void * arg) {
			struct query * rawdata;
	while( 1 ) {
			pthread_mutex_lock(&r_mutex);
			while( list_empty(&result_q)) {
				pr_debug("Fun: %s, do_notify", __FUNCTION__);
				pthread_cond_wait(&q_cond, &q_mutex);
			}
			/*now the list is not empty, and we get the lock*/
			rawdata = list_first_entry(&query_q, struct query, q_node);
			list_del(&rawdata->q_node);
			pthread_mutex_unlock(&r_mutex);
			parse_result(rawdata); /* now to parse and abstract the result*/
			notify();
		}


	return NULL;
}

/**
 * program sends out the query message, and then we make a node
 * attaching to the qurey queue, then now leave the thread to figure
 * it out!
 *
 */
void send_attach(struct query * q ) {
	if( !q->url )
		goto fail;

	/* fill the query, ok let the thread do the real_query*/
	pthread_mutex_lock(&q_mutex);
	/*do attach*/
	list_add(&q->q_node, &query_q);
	pthread_mutex_unlock(&q_mutex);

	/* informing the thread_do_query */
	pthread_cond_signal(&q_cond);
	return;
fail:
	q->flag |= QUERY_FAIL;
}

/* push the raw data */
void recieve_attach( struct query * result ) {
	pthread_mutex_lock(&r_mutex); /* lock and attach */
	list_add(&result->q_node, &result_q);
	pthread_mutex_unlock(&q_mutex);
	pthread_cond_signal(&r_cond);

}

void notify() {
	/* uh.. it's time to laungh a signal */


}

/*	init these two threads! */
void thread_init() {

}
