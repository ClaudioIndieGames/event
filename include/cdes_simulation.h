#ifndef __CDES_SIMULATION_H__
#define __CDES_SIMULATION_H__

#include "cdes_event_queue.h"

typedef struct {
    cdes_event_queue eq;
    cdes_time time;
    array threads;
    cdes_event on_simulation_start_event;
} cdes_simulation;

cdes_simulation* cdes_simulation_create(cdes_simulation* sim);
void cdes_simulation_destroy(cdes_simulation* sim);
void cdes_simulation_start(cdes_simulation* sim, char is_single_threaded);
cdes_time cdes_simulation_get_time(cdes_simulation* sim);
void cdes_simulation_event_notify(cdes_simulation* sim, cdes_event* e, cdes_time delay, void* args);

#endif  // __CDES_SIMULATION_H__