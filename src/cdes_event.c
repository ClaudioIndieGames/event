#include <assert.h>
#include "cdes_event.h"

cdes_event* cdes_event_create(cdes_event* e) {
    assert(e && "Passed NULL event");
    // allocating space for 2 tasks by default
    array_create_semi_dynamic(&e->registered_tasks, sizeof(cdes_task*), 2);
    return e;
}

void cdes_event_destroy(cdes_event* e) {
    assert(e && "Passed NULL event");
    array_destroy(&e->registered_tasks);
}

void cdes_event_register_task(cdes_event* e, cdes_task* t) {
    assert(e && "Passed NULL event");
    assert(t && "Passed NULL task");
    array_append_copy(&e->registered_tasks, &t);
}