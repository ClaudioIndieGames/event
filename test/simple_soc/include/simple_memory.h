#ifndef __SIMPLE_MEMORY_H__
#define __SIMPLE_MEMORY_H__

#include "cdes.h"

typedef struct {
    cdes_task on_receive_msg;
    cdes_event in_event;
    cdes_event* out_event;
    cdes_simulation* sim;
} simple_memory;

simple_memory* simple_memory_create(simple_memory* this, cdes_simulation* sim);
void simple_memory_destroy(simple_memory* this);

#endif  // __SIMPLE_MEMORY_H__