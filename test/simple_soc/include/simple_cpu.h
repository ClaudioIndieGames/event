#ifndef __SIMPLE_CPU_H__
#define __SIMPLE_CPU_H__

#include "cdes.h"

typedef struct {
    cdes_task on_sim_start;
    cdes_task on_receive_msg;
    cdes_event in_event;
    cdes_event* out_event;
    cdes_simulation* sim;
} simple_cpu;

simple_cpu* simple_cpu_create(simple_cpu* this, cdes_simulation* sim);
void simple_cpu_destroy(simple_cpu* this);

#endif  // __SIMPLE_CPU_H__