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

#include <result.h>
#include <json.h>
#include <malloc.h>
#include <string.h>
#include <query.h>
#include <libjs.h>
#include <kernel.h>

/* using key directly is dangerous! */
static struct server weather = {
	.url = "https://api.seniverse.com/v3/",
	.key = "ov8zy9d8r8j7grqd"
};
/**
 * 本来还想全部弄成英文，结果C/E似乎都OK
 * 最好有个 ioctl 直接对语言修改，不过这很
 * 容易实现，所以也无所谓！
 */
static struct sub_path now = {
	.api = "weather/now.json",
    .lang = "en" /* simple Chinese */
};

void struct_init() {
	weather.length = strlen(weather.key) + strlen(weather.url);
	now.length = strlen(now.api) + strlen(now.lang);
}

static inline void string_free(string * s ) {
	free(s->ptr);
	s->ptr = NULL;
}

static void free_struct(string *s , int items) {
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
static int build_string(
		json_value * parent,
		const char * childname,
		string 	* s 	) {
	debug_func();
	int retval = 0;
	json_value * var;

	var = fetch_name(entry_obj(parent).values,
			entry_obj(parent).length, childname);
	if( !var ) {
		pr_debug("%s: child no found! name: %s \n", __FUNCTION__, childname);
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
	strncpy(s->ptr, var->u.string.ptr, var->u.string.length);
	*(s->ptr + s->length) = '\0';
	pr_debug("%s : %s  length: %lu\n\n", childname, s->ptr, s->length);

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
static int  build_location( struct result * result, json_value * value ) {
	debug_func();
	int retval = 0;
	struct location * loc =
			(struct location *)malloc(sizeof(*loc));
	if( !loc ) {
		pr_debug("%s: space no enough", __FUNCTION__);
	}
	if( build_string(value, "id", &loc->id) )
		goto error;
	if( build_string(value, "name", &loc->name) )
		goto name_error;
	if ( build_string(value, "country", &loc->country) )
		goto country_error;
	if( build_string(value, "timezone", &loc->timezone) )
		goto timezone_error;
	if ( build_string(value, "timezone_offset", &loc->time_offset) )
		goto offset_error;

done:
	result->locate = loc;
	return retval;
offset_error:
	string_free(&loc->timezone);
timezone_error:
	string_free(&loc->country);
country_error:
	string_free(&loc->name);
name_error:
	string_free(&loc->id);
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
static int build_weather( struct result * ret, json_value * value ) {
	debug_func();
	int retval = 0;
	struct weather * w;
	w = (struct weather *)malloc(sizeof(*w));
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
 *	最重要的一点，这并不兼容！！！
 *	ugly code .....  but it's more logical
 *	take care of rawdata, avoid mem leak !
 */
void  build_result(struct query * result ) {
	debug_func();
	json_value * value, * child;
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
	if( !child || build_weather(res, child) )
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
	free_struct((string *)res->locate, LOCATION_ITEMS);
exit1:
	free(res);
exit:
	json_value_free(value);
	result->data = NULL;
	result->flag |= QUERY_FAIL;
}

/**
 * @result: 	contains the raw_data
 *
 * result->data is raw data, need to be parse
 * by json_paser _lib.
 */
void parse_result( struct query * result ) {
	debug_func();
	struct json_value * res = NULL;
	json_char * data = result->data; /* write back by the libcurl */
	res = json_parse(data, result->size);
	if (res == NULL) {
			fprintf(stderr, "Unable to parse data\n");
			free(data);
			result->flag |= QUERY_FAIL;
			goto done;
	}
	free(data);
	result->data = (void *)res; /* now it can be handled to build_XX Funcs */
	build_result(result);
done:
	return;
}

/**
 * 		thread_do_query will call it!
 * @q  q->url contains the location info.
 *
 *	"url/api?key=...&location=...&language="
 *
 *		string		length
 *	"location=" 		9
 *	"language="			9
 *	"key="				4
 *	"?"					1
 *	"&"	* 2				2
 *	'\0'				1
 * 					total: 26
 * 	 strlen( url api key location language );
 * 	 找不出语言形容这一段代码的丑陋了。。。。。。。。。。
 */
void build_inquiry(struct query * q ) {
	debug_func();
	int len;
	char * s, * url;
	len = 26 + weather.length + now.length + strlen(q->url);
	/* length of _url */
	url = (char *)malloc(len);
	if( !url ) {
		pr_debug("space not enough!\n");
		goto fail;
	}
	/**
	 * 	We should have checked k whether it's null every time
	 * we invoke _strchr, but i'm confident! we have already
	 * measured its length carefully! see above.
	 */
	memset(url, 0, len);
	strcpy(url, weather.url);
	s = strchr(url, 0);
	strcpy(s, now.api);
	s = strchr(s, 0);
	strcpy(s, "?");
	s++;;  /* s++ */
	strcpy(s, "key=");
	s += 4;	/* s+=4 */
	strcpy(s, weather.key);
	s = strchr(s, 0);
	strcpy(s, "&");
	s++;
	strcpy(s, "location=");
	s += 9;
	strcpy(s, q->url);
	s = strchr(s, 0);
	strcpy(s, "&");
	s++;
	strcpy(s, "language=");
	s += 9;
	strcpy(s, now.lang);
	pr_debug("%s\n", url);
	/* more sanity check */
	pr_debug("QUERY: \n url:%s \n size: %lu\n\n", q->url, q->size);
	q->url = url; /* when updating we can skip this urgly code! */
	send_query(q); /* now move to the libcurl! */
fail:
	return;
}

//int main() {
//	struct query q1 = {
//			.flag = 0,
//			.url = "sample1",
//			.size = 0,
//			.data = NULL
//		};
//	struct_init();
//	build_inquiry(&q1);
//
//	return 0;
//}

/**
 * FIXME:其实直接有一个show_string 里面传递ITEM就好了
 * 不过也无所谓，这几个函数只是为debug所用
 *
 */
void show_weather( struct weather * w) {
	int i;
	string * s = (string *)w;
	printf("\t weather: \n");
	for(i=WEATHER_ITEMS; i ; i--) {
		printf("\t\t%s\n", s->ptr);
		s++;
	}
}


void show_location( struct location * loc) {
	int i;
	string * s = (string *)loc;
	printf("\t location: \n");
	for(i=LOCATION_ITEMS; i ; i--) {
		printf("\t\t%s\n", s->ptr);
		s++;
	}
}

void show_result(struct query * q) {
	struct result * res =
			(struct result *)q->data;
	puts("results:");
	show_weather(res->now);
	show_location(res->locate);
}
