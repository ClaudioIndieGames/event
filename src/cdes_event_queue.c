#include <assert.h>
#include "cdes_event_queue.h"

cdes_event_queue* cdes_event_queue_create(cdes_event_queue* eq, size_t initial_size) {
    assert(eq && "Passed NULL event ");
    ccc_array_create(sizeof(cdes_scheduled_event), .header = &eq->scheduled_events, .elements_capacity = initial_size);
    return eq;
}

void cdes_event_queue_destroy(cdes_event_queue* eq) {
    assert(eq && "Passed NULL event queue");
    ccc_array_destroy(&eq->scheduled_events);
}

void cdes_event_queue_push(cdes_event_queue* eq, cdes_event* e, cdes_time scheduled_time, void* args) {
    assert(eq && "Passed NULL event queue");
    assert(e && "Passed NULL event");
    assert(scheduled_time >= 0 && "Passed negative time");
    cdes_scheduled_event* se = NULL;
    for (size_t i = 0; i < ccc_array_size(&eq->scheduled_events); ++i) {
        cdes_scheduled_event* other_se = ccc_array_at(&eq->scheduled_events, i);
        if (other_se->scheduled_time > scheduled_time) {
            se = ccc_array_insert(&eq->scheduled_events, i);
            return;
        }
    }
    if (se == NULL) {
        se = ccc_array_append(&eq->scheduled_events);
    }
    se->event = e;
    se->scheduled_time = scheduled_time;
    se->args = args;
}

const cdes_scheduled_event* cdes_event_queue_front(cdes_event_queue* eq) {
    assert(eq && "Passed NULL event queue");
    if (!ccc_array_empty(&eq->scheduled_events)) {
        return ccc_array_front(&eq->scheduled_events);
    }
    return NULL;
}

void cdes_event_queue_pop(cdes_event_queue* eq) {
    assert(eq && "Passed NULL event queue");
    if (!ccc_array_empty(&eq->scheduled_events)) {
        ccc_array_pop(&eq->scheduled_events);
    }
}