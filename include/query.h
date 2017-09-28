/*
 * inquiry.h
 *
 *  Created on: Sep 25, 2017
 *      Author: trance
 */

#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

#define QUERY_FAIL  (1UL)
#define QUERY_SUCCESS (1UL << 1)
#define QUERY_UPDATA (1UL << 2)

#define URL_LENGTH
#define	WEATHER_API
#define SUGGEST_API



struct query {
	unsigned int flag;
	struct list_head q_node;
	char * url;
	size_t size;
	void * data;
};

#endif /* INCLUDE_QUERY_H_ */
