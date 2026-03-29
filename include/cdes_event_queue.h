#ifndef __CDES_EVENT_QUEUE_H__
#define __CDES_EVENT_QUEUE_H__

#include "cdes_event.h"
#include "cdes_time.h"

typedef struct {
    ccc_array scheduled_events;
} cdes_event_queue;

typedef struct {
    cdes_event* event;
    cdes_time scheduled_time;
    void* args;
} cdes_scheduled_event;

cdes_event_queue* cdes_event_queue_create(cdes_event_queue* eq, size_t initial_size);
void cdes_event_queue_destroy(cdes_event_queue* eq);
void cdes_event_queue_push(cdes_event_queue* eq, cdes_event* e, cdes_time scheduled_time, void* args);
const cdes_scheduled_event* cdes_event_queue_front(cdes_event_queue* eq);
void cdes_event_queue_pop(cdes_event_queue* eq);

#endif  // __CDES_EVENT_QUEUE_H__