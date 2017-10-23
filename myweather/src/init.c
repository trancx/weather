/*
 * init.c
 *
 *  Created on: Oct 10, 2017
 *      Author: trance
 */
#include <stdio.h>
#include <query.h>
#include <json.h>
#include <kernel.h>
#include <pthread.h>
#include <init.h>
#include <queue.h>

extern int mainWidget(int argc, char * argv[]);

void init() {
    puts("init!");
	pthread_t p1, p2;
	init_curl();
	struct_init();
	thread_init();

	pthread_create(&p1, NULL, thread_do_query, NULL);
	pthread_create(&p2, NULL, thread_do_notify, NULL);
}

int main(int argc, char *argv[])
{
    puts("main func");
    init();

    return mainWidget(argc, argv);
}
