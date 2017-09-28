/*
 * libjs.c
 *
 *  Created on: Sep 24, 2017
 *      Author: trance
 */

#include <json.h>
#include <stdio.h>
#include <string.h>
#include "libjs.h"

/**
 * @parent: parent json_obj
 * @childname: the name of this json_value
 *
 *  return '@NULL' if it's no found!
 *
 *  it's because in array, no name will be repeated.
 *  so the deal to array is different!
 */


/**
 * 	a fast scan _func.  specific for json_object
 */
json_value * fetch_name( const json_object_entry * entry,
		int length, const json_char * childname) {
	json_value* ret = NULL;
	unsigned i;

	for(i=0; i < length; ++i ) {
		if( !strcmp(childname, entry[i].name) ) {
				ret = entry[i].value;
				pr_debug("%s: child found", __FUNCTION__);
				break;
			}
	}
	return ret;
}

/*
 * absolutely, functions need to be tweaked@!
 */
json_value * fetch_child( const json_value * parent ,const json_char * childname ) {
	int i;
	json_value * ret = NULL;
	json_object_entry* tmp;
	int type = parent->type;
	if( type != json_object )
		goto array;
	ret = fetch_name(entry_obj(parent).values,
			entry_obj(parent).length, childname);
array: /* this _func should be divided, like above */
	if( type != json_array )
		goto out;
	for(i=0; i < entry_array(parent).length; i++ ) {
		ret = fetch_child( entry_array(parent).values[i], childname);
		if( ret ) break;

	}
out:
	return ret;
}

/**
 *
 * just like path look_up
 * 		'\' means root json_value
 *
 */
json_value * fetch_path( const json_value * value, const json_char * path ) {

}






