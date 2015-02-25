#ifndef _MANTIS_H
#define _MANTIS_H 1

#define __DEBUG 1


#ifdef __DEBUG
# define DEBUG(fmt, ...) printk(fmt, ##__VA_ARGS__)
#else
# define DEBUG(fmt, ...)
#endif






#endif
