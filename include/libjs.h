/*
 * libjs.h
 *
 *  Created on: Sep 25, 2017
 *      Author: trance
 */

#ifndef INCLUDE_LIBJS_H_
#define INCLUDE_LIBJS_H_

#define entry_obj(obj) ((obj)->u.object)
#define entry_obj_entry(obj) ((obj)->u.object)
#define entry_array(a) ((a)->u.array)
#define entry_string(s) ((s)->u.string)

extern json_value * fetch_name( const json_object_entry * entry,
		int length, const json_char * childname);

extern json_value * fetch_child( const json_value * parent ,
		const json_char * childname );

#endif /* INCLUDE_LIBJS_H_ */
