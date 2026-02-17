#ifndef __EVENT_H__
#define __EVENT_H__

#include <assert.h>
#include <pthread.h>
#include "array/array.h"

typedef void* (*thread_func_t)(void*);
typedef size_t simulation_time_t;

typedef struct {
    thread_func_t func;
    void* module;
    size_t priority;
} task;

typedef struct {
    array funcs;
    pthread_t id;
} thread;

typedef struct {
    array registered_tasks;
} event;

typedef struct {
    event* event;
    simulation_time_t scheduled_time;
} scheduled_event;

typedef struct {
    array scheduled_events;
    array threads;
} event_scheduler;

typedef struct {
    event_scheduler scheduler;
    simulation_time_t time;
} simulation;

task* task_create(task* t, thread_func_t func, void* module, size_t priority) {
    assert(t && "Passed NULL task");
    assert(func && "Passed NULL function pointer");
    assert(module && "Passed NULL module");
    t->func = func;
    t->module = module;
    t->priority = priority;
    return t;
}

void task_destroy(task* t) {
    assert(t && "Passed NULL task");
    // nop
}

thread* thread_create(thread* t) {
    assert(t && "Passed NULL thread");
    // allocating space for 2 func by default
    array_create_semi_dynamic(&t->funcs, sizeof(thread_func_t), 2);
    return t;
}

void thread_destroy(thread* t, bool join) {
    assert(t && "Passed NULL thread");
    if (join) {
        pthread_join(t->id, NULL);
    }
    array_destroy(&t->funcs);
}

void* thread_main(void* arg) {
    array* funcs = arg;
    for (size_t i = 0; i < array_size(funcs); ++i) {
        thread_func_t func = *(thread_func_t*)array_at(funcs, i);
        func(NULL);
    }
    return NULL;
}

void thread_run(thread* t, bool single_threaded) {
    if (!single_threaded) {
        pthread_create(&t->id, NULL, thread_main, &t->funcs);
    } else {
        thread_main(&t->funcs);
    }
}

event* event_create(event* e) {
    assert(e && "Passed NULL event");
    // allocating space for 2 tasks by default
    array_create_semi_dynamic(&e->registered_tasks, sizeof(task*), 2);
    return e;
}

void event_destroy(event* e) {
    assert(e && "Passed NULL event");
    array_destroy(&e->registered_tasks);
}

void insert_task_sorted(array* a, task* t) {
    assert(a && "Passed NULL array");
    assert(t && "Passed NULL task");
    size_t pos = array_size(a);
    for (size_t i = 0; i < array_size(a); ++i) {
        task* tx = *(task**)array_at(a, i);
        if (tx->module < t->module) {
            pos = i;
            break;
        } else if (tx->module == t->module) {
            if (tx->priority <= t->priority) {
                pos = i;
                break;
            }
        }
    }
    array_insert_copy(a, &t, pos);
}

void event_register_task(event* e, task* t) {
    assert(e && "Passed NULL event");
    insert_task_sorted(&e->registered_tasks, t);
}

scheduled_event* scheduled_event_create(scheduled_event* e, event* event, simulation_time_t scheduled_time) {
    assert(e && "Passed NULL scheduled event");
    e->event = event;
    e->scheduled_time = scheduled_time;
    return e;
}

void scheduled_event_destroy(scheduled_event* e) {
    assert(e && "Passed NULL scheduled event");
    // nop
}

void event_notify(simulation* sim, event* e, simulation_time_t delay) {
    assert(sim && "Passed NULL simulation");
    assert(e && "Passed NULL event");
    assert(delay && "Cannot notify with zero delay");
    simulation_time_t scheduled_time = delay + sim->time;
    for (size_t i = 0; i < array_size(&sim->scheduler.scheduled_events); ++i) {
        scheduled_event* se = array_at(&sim->scheduler.scheduled_events, i);
        if (se->scheduled_time > scheduled_time) {
            scheduled_event* new_se = array_insert_slot(&sim->scheduler.scheduled_events, i);
            scheduled_event_create(new_se, e, scheduled_time);
            return;
        }
    }
    scheduled_event* new_se = array_append_slot(&sim->scheduler.scheduled_events);
    scheduled_event_create(new_se, e, scheduled_time);
}

event_scheduler* event_scheduler_create(event_scheduler* s) {
    assert(s && "Passed NULL event scheduler");
    // allocating space for 10 events by default
    array_create_semi_dynamic(&s->scheduled_events, sizeof(scheduled_event), 10);
    // allocating space for 2 threads by default
    array_create_semi_dynamic(&s->threads, sizeof(thread), 2);
    return s;
}

void event_scheduler_destroy(event_scheduler* s) {
    assert(s && "Passed NULL event scheduler");
    for (size_t i = 0; i < array_size(&s->scheduled_events); ++i) {
        scheduled_event_destroy(array_at(&s->scheduled_events, i));
    }
    array_destroy(&s->scheduled_events);
    if (!array_empty(&s->threads)) {
        assert(0 && "Threads array should be empty");
    }
    array_destroy(&s->threads);
}

scheduled_event* event_scheduler_front(event_scheduler* s) {
    assert(s && "Passed NULL event scheduler");
    if (!array_empty(&s->scheduled_events)) {
        return array_front(&s->scheduled_events);
    }
    return NULL;
}

scheduled_event* event_scheduler_next(event_scheduler* s) {
    assert(s && "Passed NULL event scheduler");
    if (!array_empty(&s->scheduled_events)) {
        array_pop(&s->scheduled_events);
        return event_scheduler_front(s);
    }
    return NULL;
}

simulation* simulation_create(simulation* sim) {
    assert(sim && "Passed NULL simulation");
    event_scheduler_create(&sim->scheduler);
    sim->time = 0;
    return sim;
}

void simulation_destroy(simulation* sim) {
    assert(sim && "Passed NULL simulation");
    event_scheduler_destroy(&sim->scheduler);
}

void simulation_run(simulation* sim, bool single_threaded, array* sorted_tasks) {

    // create threads
    thread* th;
    void* prev_module = NULL;
    for (size_t i = 0; i < array_size(sorted_tasks); ++i) {
        task* t = *(task**)array_at(sorted_tasks, i);
        if (prev_module != t->module) {
            th = array_append_slot(&sim->scheduler.threads);
            thread_create(th);
            prev_module = t->module;
        }
        array_append_copy(&th->funcs, &t->func);
    }

    // call threads
    for (size_t i = 0; i < array_size(&sim->scheduler.threads); ++i) {
        thread* t = array_at(&sim->scheduler.threads, i);
        thread_run(t, single_threaded);
    }

    // destroy threads
    for (size_t i = 0; i < array_size(&sim->scheduler.threads); ++i) { 
        thread* t = array_at(&sim->scheduler.threads, i);
        thread_destroy(t, !single_threaded);
    }
    array_clear(&sim->scheduler.threads);

}

void simulation_start(simulation* sim, bool single_threaded) {
    assert(sim && "Passed NULL simulation");

    // array storing all the task pointers to call at a specific time, initial allocate of 10 elements
    array* sorted_tasks = array_create_semi_dynamic(&(array){0}, sizeof(task*), 10);

    // initialize sim time
    scheduled_event* se = event_scheduler_front(&sim->scheduler);
    if (se != NULL) { sim->time = se->scheduled_time; }
    while (se != NULL) {
        
        // merge the tasks of each event using the sorted_tasks array
        array* rt = &se->event->registered_tasks;
        for (size_t i = 0; i < array_size(rt); ++i) {
            task* t = *(task**)array_at(rt, i);
            insert_task_sorted(sorted_tasks, t);
        }

        se = event_scheduler_next(&sim->scheduler);

        // collect tasks until the sim time changes
        if (se == NULL || sim->time != se->scheduled_time) {

            // run tasks
            simulation_run(sim, single_threaded, sorted_tasks);
            array_clear(sorted_tasks);
            // after the run some events might have been triggered
            if (se == NULL) { se = event_scheduler_front(&sim->scheduler); }
            
            // update time
            if (se != NULL) { sim->time = se->scheduled_time; }
        }
    }

    array_destroy(sorted_tasks);
}

#endif  // __EVENT_H__