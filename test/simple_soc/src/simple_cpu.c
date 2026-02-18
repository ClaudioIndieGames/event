#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "simple_cpu.h"
#include "simple_message.h"

void* simple_cpu_on_sim_start(void* module, void* args) {
    (void)args;
    simple_cpu* this = module;
    printf("[%ld, %ld, CPU] Sending 'Hi Memory' in 10\n", cdes_simulation_get_time(this->sim), (unsigned long)pthread_self());
    simple_message* msg = simple_message_create(20);
    msg->from = this;
    strcpy(msg->data, "Hi Memory\0");
    cdes_simulation_event_notify(this->sim, this->out_event, 10, msg);
    return NULL;
}

void* simple_cpu_on_receive_msg(void* module, void* args) {
    simple_cpu* this = module;
    simple_message* msg = args;
    printf("[%ld, %ld, CPU] Received '%s' from %p\n", cdes_simulation_get_time(this->sim), (unsigned long)pthread_self(), msg->data, msg->from);
    simple_message_destroy(msg);
    return NULL;
}

simple_cpu* simple_cpu_create(simple_cpu* this, cdes_simulation* sim) {
    cdes_task_create(&this->on_sim_start, this, simple_cpu_on_sim_start, 1);
    cdes_task_create(&this->on_receive_msg, this, simple_cpu_on_receive_msg, 0);
    cdes_event_create(&this->in_event);
    this->sim = sim;
    cdes_event_register_task(&sim->on_simulation_start_event, &this->on_sim_start);
    cdes_event_register_task(&this->in_event, &this->on_receive_msg);
    return this;
}

void simple_cpu_destroy(simple_cpu* this) {
    cdes_task_destroy(&this->on_sim_start);
    cdes_task_destroy(&this->on_receive_msg);
    cdes_event_destroy(&this->in_event);
}