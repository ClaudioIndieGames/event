#include <assert.h>
#include "cdes_event.h"

cdes_event* cdes_event_create(cdes_event* e) {
    assert(e && "Passed NULL event");
    // allocating space for 1 tasks by default
    ccc_array_create(sizeof(cdes_task), .header = &e->registered_tasks, .elements_capacity = 1);
    return e;
}

void cdes_event_destroy(cdes_event* e) {
    assert(e && "Passed NULL event");
    ccc_array_destroy(&e->registered_tasks);
}

void cdes_event_register_task(cdes_event* e, cdes_task* t) {
    assert(e && "Passed NULL event");
    assert(t && "Passed NULL task");
    cdes_task* task = ccc_array_append(&e->registered_tasks);
    *task = *t;
}