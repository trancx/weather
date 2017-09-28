/*
 * result.h
 *
 *  Created on: Sep 24, 2017
 *      Author: trance
 */

#ifndef INCLUDE_RESULT_H_
#define INCLUDE_RESULT_H_

#include "list.h"

#define LOCATION_ITEMS 5
#define WEATHER_ITEMS 3
//#define SUGGESTION_ITEMS

typedef struct {
	unsigned int length;
	char * ptr;
}string;


struct location {
	string id;
	string name;
	string country;
	string timezone;
	string time_offset;
};

struct weather {
	string text;
	string code;
	string temperature;
};
#ifdef __SUGGESTION__
struct instruct {
	string name;
	string belief;
	string details;
};
#endif

struct suggest {
	unsigned int length;
	struct instruct * d;
};

struct result {
	struct location * locate;		/* location */
	struct weather * now;			/* weather now */
	#ifdef __SUGGESTION__
	struct suggest * sugs;		/* 	suggestion if exist*/
	#endif
};





#endif /* INCLUDE_RESULT_H_ */
