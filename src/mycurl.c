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
#include "query.h"


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
	size_t realsize = size * nmemb;
	struct query * result = (struct query *)userp;
	result->data = malloc(realsize);
	if( !result ) {
		pr_debug("%s malloc: memory not enough", __FUNCTION__);
		goto fail;
	}
	memcpy(result, content, realsize);
	result->size = realsize;
	recieve_attach(result);
	return realsize;
fail:
	return 0;
}


/**
 * send the actial request using (libcurl)http:get
 *
 * @url:    	search the result
 * @userp: 		space the data copied to
 *
 */
void send_query(const char * url, void * userp ) {
	CURL * curl_handle;
	CURLcode res;

//	curl_global_init(CURL_GLOBAL_ALL);
//
//	curl_handle = curl_easy_init();
//
//	curl_easy_setopt(curl_handle, CURLOPT_URL, url );
//
//	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, result_callback);
//
}

