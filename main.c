#include <stdio.h>
#include "event.h"

// scheduler
event_scheduler s;

// events
event e1;
event e2;

// tasks
void task1() {
    printf("[%lds] Hi from task1\n", s.sim_time);
}

void task3() {
    printf("[%lds] Hi from task3\n", s.sim_time);
}

void task2() {
    printf("[%lds] Hi from task2\n", s.sim_time);
    event_register_task(&e2, task3);
    event_scheduler_schedule(&s, &e2, 10);
}

int main() {
    // constructor
    event_scheduler_create(&s);
    event_create(&e1);
    event_create(&e2);

    event_register_task(&e1, task1);
    event_register_task(&e1, task2);
    event_scheduler_schedule(&s, &e1, 10);

    event_scheduler_run(&s);

    // destructor
    event_destroy(&e1);
    event_destroy(&e2);
    event_scheduler_destroy(&s);

    return 0;
}