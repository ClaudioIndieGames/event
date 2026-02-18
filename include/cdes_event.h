#ifndef __CDES_EVENT_H__
#define __CDES_EVENT_H__

#include "array.h"
#include "cdes_task.h"

typedef struct {
    array registered_tasks;
} cdes_event;

cdes_event* cdes_event_create(cdes_event* e);
void cdes_event_destroy(cdes_event* e);
void cdes_event_register_task(cdes_event* e, cdes_task* t);

#endif  // __CDES_EVENT_H__