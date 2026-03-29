#include <assert.h>
#include "cdes_thread.h"

cdes_thread* cdes_thread_create(cdes_thread* t) {
    assert(t && "Passed NULL thread");
    // allocating space for 1 func by default
    ccc_array_create(sizeof(cdes_thread_func), .header = &t->funcs, .elements_capacity = 1);
    return t;
}

void cdes_thread_destroy(cdes_thread* t, cdes_simulation_mode mode) {
    assert(t && "Passed NULL thread");
    if (mode == CDES_MULTI_THREADED) {
        pthread_join(t->id, NULL);
    }
    for (size_t i = 0; i < ccc_array_size(&t->funcs); ++i) {
        cdes_thread_func* tf = ccc_array_at(&t->funcs, i);
        cdes_thread_func_destroy(tf);
    }
    ccc_array_destroy(&t->funcs);
}

void* cdes_thread_main(void* args) {
    ccc_array* funcs = args;
    for (size_t i = 0; i < ccc_array_size(funcs); ++i) {
        cdes_thread_func* tf = ccc_array_at(funcs, i);
        cdes_task_func func = cdes_task_get_func(tf->task);
        func(tf->task->module, tf->args);
    }
    return NULL;
}

void cdes_thread_run(cdes_thread* t, cdes_simulation_mode mode) {
    if (mode == CDES_MULTI_THREADED) {
        pthread_create(&t->id, NULL, cdes_thread_main, &t->funcs);
    } else {
        cdes_thread_main(&t->funcs);
    }
}

cdes_thread_func* cdes_thread_func_create(cdes_thread_func* tf, cdes_task* t, void* args) {
    assert(tf && "Passed NULL thread function");
    assert(t && "Passed NULL task");
    tf->task = t;
    tf->args = args;
    return tf;
}

void cdes_thread_func_destroy(cdes_thread_func* tf) {
    assert(tf && "Passed NULL thread function");
    // nop
}