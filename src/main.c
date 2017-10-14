/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: trance
 */
#include <stdio.h>
#include <query.h>
#include "json.h"
#include <kernel.h>
#include <pthread.h>
#include <init.h>
#include <queue.h>



void init() {
	init_curl();
	struct_init();
	thread_init();
}

int main(int argc, char * argv [] ) {
	pthread_t p1, p2;

	struct query * q =
			(struct query *)malloc(sizeof(*q));
	q->url = argv[1];

	init();
	pthread_create(&p1, NULL, thread_do_query, NULL);
	pthread_create(&p2, NULL, thread_do_notify, NULL);

	send_attach(q);

	sleep(10);

	printf("\t\tupdating:\n");
	q->flag &= 0;
	q->flag |= QUERY_UPDATE;
	send_attach(q);

	sleep(2);
	free(q);
	return 0;
}
