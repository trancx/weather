/*
 * mycurl.c
 *
 *	most part of things are taken care by libcurl
 *	i just do the easiest job!
 *
 *
 *  Created on: Sep 25, 2017
 *      Author: trance
 */
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <query.h>
#include <json.h>
#include <kernel.h>
#include <pthread.h>
#include <init.h>
#include <queue.h>
#include <string.h>


static CURL * curl_handle;

/* _incase of race condition */
pthread_mutex_t curl_lock;
pthread_mutexattr_t c_attr;

/**
 * call back needed for libcurl
 *
 * @content: the raw data recieved
 * @size:    the size
 * @nmemb:   number of memmory block
 * @userp:   the data we copy to
 *
 */
static size_t
result_callback(void *content, size_t size,
		size_t nmemb, void * userp) {
	debug_func();
	size_t realsize = size * nmemb;
	struct query * result = (struct query *)userp;
	result->data = realloc(result->data,result->size + realsize);
	if( !result ) {
		printf("%s malloc: memory not enough", __FUNCTION__);
		goto fail;
	}
	memcpy(result->data, content, realsize);
	result->size = realsize;
	pr_debug("%s size: %lu\n ", __FUNCTION__, realsize);
	recieve_attach(result);
	return realsize;
fail:
	return 0;
}


/**
 * send the actual request using (libcurl)http:get
 *
 * @url:    	search the result
 * @userp: 		space the data copied to
 *
 *	FIXME: I think the curl_handle should be a global,
 *	or static, it means it should be initialized once
 *	instead of every time we provoke the send_query
 */
void send_query(struct query * q ) {
	debug_func();
	CURLcode res;
	pthread_mutex_lock(&curl_lock);
	curl_global_init(CURL_GLOBAL_ALL);

	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle,
			CURLOPT_URL, q->url );

	curl_easy_setopt(curl_handle,
				CURLOPT_WRITEFUNCTION, result_callback);

	curl_easy_setopt(curl_handle,
			CURLOPT_WRITEDATA, (void *)q );

		/* get it! */
	res = curl_easy_perform(curl_handle);

	  /* check for errors */
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
	    curl_easy_strerror(res));
	 } else {
		 pr_debug("received size: %lu\n", ((struct query*)q)->size);
	 }

	curl_easy_cleanup(curl_handle);
	/**
	 * FIXME: I don't know what curl_clean_up() really does,
	 * or we can move more set_opt_funcs to the init_curl()
	 * (see _below) instead of here.
	 *
	 */
	curl_global_cleanup();
	pthread_mutex_unlock(&curl_lock);
}

/**
 * FIXME: should test the return value of thread_init
 */
void init_curl() {
	debug_func();
	pthread_mutexattr_init(&c_attr);
	pthread_mutexattr_settype(&c_attr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&curl_lock, &c_attr);
}

void clean_curl() {

}



