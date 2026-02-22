#include <assert.h>
#include "cdes_event_queue.h"

cdes_event_queue* cdes_event_queue_create(cdes_event_queue* eq, size_t initial_size) {
    assert(eq && "Passed NULL event ");
    array_create_semi_dynamic(&eq->scheduled_events, sizeof(cdes_scheduled_event), initial_size);
    return eq;
}

void cdes_event_queue_destroy(cdes_event_queue* eq) {
    assert(eq && "Passed NULL event queue");
    for (size_t i = 0; i < array_size(&eq->scheduled_events); ++i) {
        cdes_scheduled_event_destroy(array_at(&eq->scheduled_events, i));
    }
    array_destroy(&eq->scheduled_events);
}

void cdes_event_queue_push_copy(cdes_event_queue* eq, cdes_scheduled_event* se) {
    assert(eq && "Passed NULL event queue");
    assert(se && "Passed NULL scheduled event");
    for (size_t i = 0; i < array_size(&eq->scheduled_events); ++i) {
        cdes_scheduled_event* other_se = array_at(&eq->scheduled_events, i);
        if (cdes_scheduled_event_get_time(other_se) > cdes_scheduled_event_get_time(se)) {
            array_insert_copy(&eq->scheduled_events, se, i);
            return;
        }
    }
    array_append_copy(&eq->scheduled_events, se);
}

cdes_scheduled_event* cdes_event_queue_front(cdes_event_queue* eq) {
    assert(eq && "Passed NULL event queue");
    if (!array_empty(&eq->scheduled_events)) {
        return array_front(&eq->scheduled_events);
    }
    return NULL;
}

void cdes_event_queue_pop(cdes_event_queue* eq) {
    assert(eq && "Passed NULL event queue");
    if (!array_empty(&eq->scheduled_events)) {
        array_pop(&eq->scheduled_events);
    }
}

cdes_scheduled_event* cdes_scheduled_event_create(cdes_scheduled_event* se, cdes_event* e, cdes_time scheduled_time, void* args) {
    assert(se && "Passed NULL scheduled event");
    assert(e && "Passed NULL event");
    assert(scheduled_time >= 0 && "Passed negative time");
    se->event = e;
    se->scheduled_time = scheduled_time;
    se->args = args;
    return se;
}

void cdes_scheduled_event_destroy(cdes_scheduled_event* se) {
    assert(se && "Passed NULL scheduled event");
    // nop
}

cdes_time cdes_scheduled_event_get_time(cdes_scheduled_event* se) {
    assert(se && "Passed NULL scheduled event");
    return se->scheduled_time;
}