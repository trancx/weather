/*
 * queue.c
 *
 *	we attach requests to the list, to make an inquiry
 *	when invoking inquiry(), then the query will
 *	make a real query!
 *
 *	and attach result to the list, when received
 *	result, and call notify() to launch an event
 *
 *	mutex_lock in case of race condition!
 *
 *
 *
 *  Created on: Sep 24, 2017
 *      Author: trance
 */
#include <pthread.h>
#include <kernel.h>
#include <query.h>
#include <list.h>
#include <stdio.h>
#include <queue.h>
#include <malloc.h>

LIST_HEAD(query_q);
LIST_HEAD(result_q);

extern void notifyWeather(struct query * q);

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
	debug_func();
	while( 1 ) {
		pthread_mutex_lock(&q_mutex);
		while( list_empty(&query_q)) {
			pr_debug("query_list is empty! thread_sleep\n");
			pthread_cond_wait(&q_cond, &q_mutex);
		}
		/*now the list if not empty, and we get the lock*/
		q = list_first_entry(&query_q, struct query, q_node);
		list_del(&q->q_node);
		pr_debug("%s: unload the query!\n", __FUNCTION__ );
		if( q->flag & QUERY_UPDATE) {
			pr_debug("%s: doing update\n", __FUNCTION__);
			free(q->data);
			q->data = NULL;
			q->size = 0;
			send_query(q);
		} else {
			build_inquiry(q); /* now to build the request*/
		}
		pthread_mutex_unlock(&q_mutex); /* unlock */
	}


	return NULL;
}

void * thread_do_notify( void * arg) {
		struct query * rawdata;
		debug_func();
	while( 1 ) {
			pthread_mutex_lock(&r_mutex);
			while( list_empty(&result_q)) {
				pr_debug("result_list is empty! thread_sleep\n");
				pthread_cond_wait(&r_cond, &r_mutex);
			}
			/*now the list is not empty, and we get the lock*/
			pr_debug("%s: unload the result", __FUNCTION__ );
			rawdata = list_first_entry(&result_q, struct query, q_node);
			list_del(&rawdata->q_node);
			pthread_mutex_unlock(&r_mutex);
			pr_debug("%s: result url: %s\n size: %lu", __FUNCTION__,
					rawdata->url, rawdata->size);
			parse_result(rawdata); /* now to parse and abstract the result*/
            notifyWeather(rawdata);
		}


	return NULL;
}

/**
 * program sends out the query message, and then we make a node
 * attaching to the query queue, then now leave the thread to figure
 * it out!
 *
 * D：为了使主程序更快响应界面，我们只要求它把一些基本的数据(查找的城市)填写，而不是
 * 填写之后还得等待，所以添加到queue中就可以返回了，剩下的交由后台的线程完成更细致的操作。
 *
 */
void send_attach(struct query * q ) {
	debug_func();
	if( !q->url )

		goto fail;
	/* fill the query, ok let the thread do the real_query*/
	pthread_mutex_lock(&q_mutex);
	pr_debug("%s: query list locked!!\n", __FUNCTION__ );
	/*do attach*/
	list_add(&q->q_node, &query_q);
	pthread_mutex_unlock(&q_mutex);
	pr_debug("%s: query list unlocked!!\n", __FUNCTION__);

	/* informing the thread_do_query */
	pthread_cond_signal(&q_cond);
	return;
fail:
	q->flag |= QUERY_FAIL;
}

/* push the raw data */
void recieve_attach( struct query * result ) {
	debug_func();
	pthread_mutex_lock(&r_mutex); /* lock and attach */
	list_add(&result->q_node, &result_q);
	pthread_mutex_unlock(&r_mutex);
	pthread_cond_signal(&r_cond);

}

/**
void notify(struct query * res) {
	/* uh.. it's time to laungh a signal 
	debug_func();
	if( res->flag & QUERY_SUCCESS) {
		show_result(res);
	} else {
		puts("fail :-< ");
	}

}
 */

/*	init these two threads! */
void thread_init() {
	debug_func();
	pthread_mutexattr_init(&q_attr);
	pthread_mutexattr_init(&r_attr);

	pthread_mutexattr_settype(&q_attr, PTHREAD_MUTEX_NORMAL);
	pthread_mutexattr_settype(&r_attr, PTHREAD_MUTEX_NORMAL);

	pthread_mutex_init(&q_mutex, &q_attr);
	pthread_mutex_init(&r_mutex, &r_attr);

}



//int main() {
//	pthread_t t1;
//	struct query q1 = {
//		.flag = 0,
//		.url = "sample1",
//		.size = 0,
//		.data = NULL
//	};
//	INIT_LIST_HEAD(&q1.q_node);
//
//	struct query q2 = {
//			.flag = 0,
//			.url = "sample2",
//			.size = 0,
//			.data = NULL
//		};
//	INIT_LIST_HEAD(&q2.q_node);
//
//	thread_init();
//
//	pthread_create(&t1, NULL, thread_do_query, NULL);
//	while( 1 ) {
//		send_attach(&q1);
//		send_attach(&q2);
//
//		sleep(2);
//
//
//	}
//
//	return 0;
//}
