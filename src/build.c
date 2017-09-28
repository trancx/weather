/*
 * build.c
 *
 * 	abbstraction implementation
 * 		from .json to result struct
 * 		from user_input to url
 *
 *  Created on: Sep 25, 2017
 *      Author: trance
 */

#include "result.h"
#include "json.h"
#include <malloc.h>
#include <string.h>
#include "query.h"
#include "libjs.h"


json_value * fetch_child( const json_value *  ,const json_char * );

inline void string_free(string * s ) {
	free(s->ptr);
	s->ptr = NULL;
}

void free_strcuct(string *s , int items) {
	int i;
	for ( i=0; i<items; ++i) {
		string_free(s);
		s++;
	}
}

/**
 * 	here we use a quick scan
 *
 * 	@parent: 	obj where we could find "childname"
 * 	@childname:	child's name
 * 	@s:  		string we build for
 */
int build_string(
		json_value * parent,
		const char * childname,
		string 	* s 	) {
	debug_func();
	int retval = 0;
	json_value * var;

	var = fetch_name(entry_obj(parent).values,
			entry_obj(parent).length, childname);
	if( !var ) {
		pr_debug("%s: child no found! ", __FUNCTION__);
		goto no_found;
	}
	if( var->type != json_string ) {
		pr_debug("%s: child is no a string", __FUNCTION__);
		goto type_error;
	}
	s->length = var->u.string.length;
	s->ptr = (json_char *)malloc(s->length + 1);
	if( !s->ptr ) {
		pr_debug("%s: space not enough! ", __FUNCTION__);
	}
	strcpy(s->ptr, var->u.string.ptr);

	return retval;
malloc_error:
type_error:
no_found:
	retval = -1;
	return retval;
}

/**
 * @ret:  	result struct
 * @value:  location: { ... }
 */
int  build_location( struct result * result, json_value * value ) {
	debug_func();
	int retval = 0;
	struct location * loc =
			(struct location *)malloc(*loc);
	if( !loc ) {
		pr_debug("%s: space no enough", __FUNCTION__);
	}
	if( build_string(loc, "id", &loc->id) )
		goto error;
	if( build_string(loc, "name", &loc->name) )
		goto name_error;
	if ( build_string(loc, "country", &loc->country) )
		goto country_error;
	if( build_string(loc, "timezone", &loc->timezone) )
		goto timezone_error;
	if ( build_string(loc, "timezone_offset", &loc->time_offset) )
		goto offset_error;

done:
	result->locate = loc;
	return retval;
offset_error:
	string_free(loc->timezone);
timezone_error:
	string_free(loc->country);
country_error:
	string_free(loc->name);
name_error:
	string_free(loc->id);
id_error:
	free(loc);
	result->locate = NULL;
error:
	retval = -1;
	return retval;
}

/**
 * @ret: 	result struct we build for
 * @value:	now: { }
 */
int build_weather( struct result * ret, json_value * value ) {
	debug_func();
	int retval = 0;
	struct weather * w;
	w = (struct weather *)malloc(*w);
	if( !w ) {
		pr_debug("%s: spaces no enough");
		goto malloc_error;
	}
	if( build_string(value, "text", &w->text) )
		goto text_error;

	if( build_string(value, "code", &w->code) )
		goto code_error;

	if( build_string(value, "temperature", &w->temperature))
		goto tem_error;
done:
	ret->now = w;
	return retval;

tem_error:
	string_free(&w->code);
code_error:
	string_free(&w->text);
text_error:
	free(w);
malloc_error:
	ret->now = NULL;
	return retval;

}

#ifdef __SUGGESTION__
int build_suggestion( struct result * ret, json_value * value ) {
	int retval = 0;


	return retval;
}
#endif

/**
 * @result: 	contains the  data
 *
 *
 *	ugly code .....  but it's more logical
 *	take care of rawdata, avoid mem leak !
 */
void  build_result(struct query * result ) {
	debug_func();
	json_value value, child;
	value = result->data;
	if( !(value = fetch_child(value, "results")) )
			goto exit;
	struct result * res =
			(struct result *)malloc(sizeof(*res));
	if( !res ) {
		pr_debug("%s: space not enough");
		goto exit;
	}
	child = fetch_child(value, "location");
	if( !child || build_location(res, child) )
		goto exit1;

	child = fetch_child(value, "now");
	if( !child || build_weather(res, value) )
		goto exit2;

	#ifdef __SUGGESTION__
	child = fetch_child(value, "suggestion")
	if( !child || build_suggestion(res, value) )
		goto exit3;
	#endif
done:
	result->data = res;
	result->flag |= QUERY_SUCCESS;
	json_value_free(value);
	return;
#ifdef __SUGGESTION__
exit3:
	free_struct((string)*res->weather, WEATHER_ITEMS);
#endif
exit2:
	free_strcuct((string *)res->locate, LOCATION_ITEMS);
exit1:
	free(res);
exit:
	json_value_free(value);
	result->data = NULL;
	result->flag |= QUERY_FAIL;
}

void parse_result( struct query * result ) {
	debug_func();
	struct json_value * var = NULL;
	json_char * data = result->data; /* write back by the libcurl */
	var = json_parse(data, result->size);
	result->data = (void *)var; /* now it can be handled to build_XX Funcs */
	free(data);
	build_result(result);
}

/* thread_do_query will call it! */
void build_inquiry(struct query * q ) {
	debug_func();
	int length = strlen(q->url);
	char * url = (char *)malloc(); /*	build the correct url, to get*/

	/* more sanity check */

	send_query(url, q); /* now handling to the libcurl! */
}

