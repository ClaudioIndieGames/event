#include <stdio.h>
#include <assert.h>
#include "cdes_time.h"

cdes_time_string* cdes_time_to_string(cdes_time t, cdes_time_string* s) {
    assert(t >= 0 && "Passed negative time");

    if (t >= 1) {
        snprintf(*s, sizeof(cdes_time_string), "%.3gs", t);
    } else if (t >= 1e-3) {
        snprintf(*s, sizeof(cdes_time_string), "%.3gms", t * 1e3);
    } else if (t >= 1e-6) {
        snprintf(*s, sizeof(cdes_time_string), "%.3gus", t * 1e6);
    } else if (t >= 1e-9) {
        snprintf(*s, sizeof(cdes_time_string), "%.3gns", t * 1e9);
    } else if (t >= 1e-12) {
        snprintf(*s, sizeof(cdes_time_string), "%.3gps", t * 1e12);
    } else if (t >= 1e-15) {
        snprintf(*s, sizeof(cdes_time_string), "%.3gfs", t * 1e15);
    } else {
        snprintf(*s, sizeof(cdes_time_string), "%.3gs", t);
    }
    
    return s;
}