#ifndef __CDES_TIME_H__
#define __CDES_TIME_H__

typedef double cdes_time;
typedef char cdes_time_string[12];

cdes_time_string* cdes_time_to_string(cdes_time t, cdes_time_string* s);

#endif  // __CDES_TIME_H__