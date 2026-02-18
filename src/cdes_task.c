#include <assert.h>
#include "cdes_task.h"

cdes_task* cdes_task_create(cdes_task* t, void* module, cdes_task_func func, size_t priority) {
    assert(t && "Passed NULL task");
    assert(module && "Passed NULL module");
    assert(func && "Passed NULL function");
    t->module = module;
    t->func = func;
    t->priority = priority;
    return t;
}

void cdes_task_destroy(cdes_task* t) {
    assert(t && "Passed NULL task");
    // nop
}

cdes_task_func cdes_task_get_func(cdes_task* t) {
    assert(t && "Passed NULL task");
    return t->func;
}