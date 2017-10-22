/*
 * query.h
 *
 *  Created on: Sep 25, 2017
 *      Author: trance
 */

#ifndef INCLUDE_QUERY_H_
#define INCLUDE_QUERY_H_

//#include <list.h>

#define QUERY_FAIL  (1UL)
#define QUERY_SUCCESS (1UL << 1)
#define QUERY_UPDATE (1UL << 2)

#define URL_LENGTH
#define	WEATHER_API
#define SUGGEST_API


/* for cpp to invoke */
extern void send_attach(struct query *);



/* that's why we can access the remote! */
struct server {
	const char * url;
	const char * key;
	unsigned long length;
};

/* sub path of server */
struct sub_path {
	const char * lang;
	const char * api;
	unsigned long length;
};

struct query {
	unsigned int flag;
    /* struct list_head q_node;*/
	char * url;
	size_t size;
	void * data;
    void * arg;
};

	/*  mycurl.c */
extern void send_query(struct query *);

	/* build.c */
extern void build_inquiry(struct query *);
extern void build_result(struct query *);
extern void parse_result(struct query *);
extern void show_result(struct query *);


#endif /* INCLUDE_QUERY_H_ */
