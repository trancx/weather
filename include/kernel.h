/*
 * 'kernel.h' contains some often-used function prototypes etc
 *
 *
 *
 *  	i take it from linux-kernel!  --trance
 */
#ifndef __KERNEL__
#define __KERNEL__

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})



#ifdef DEBUG
#define pr_debug(fmt,arg...) \
	printf(fmt,##arg)
#else
#define pr_debug(fmt,arg...) \
	do { } while (0)
#endif

#define debug_func() \
	pr_debug("in function: %s\n", __FUNCTION__)

#endif
