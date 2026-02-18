#ifndef __CDES_TASK_H__
#define __CDES_TASK_H__

#include <stddef.h>

typedef void* (*cdes_task_func)(void*, void*);

typedef struct {
    void* module;
    cdes_task_func func;
    size_t priority;
} cdes_task;

cdes_task* cdes_task_create(cdes_task* t, void* module, cdes_task_func func, size_t priority);
void cdes_task_destroy(cdes_task* t);
cdes_task_func cdes_task_get_func(cdes_task* t);

#endif  // __CDES_TASK_H__