#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "simple_memory.h"
#include "simple_message.h"

void* simple_memory_on_receive_msg(void* module, void* args) {
    simple_memory* this = module;
    simple_message* msg = args;
    printf("[%ld, %ld, Memory] Received '%s' from %p\n", cdes_simulation_get_time(this->sim), (unsigned long)pthread_self(), msg->data, msg->from);
    printf("[%ld, %ld, Memory] Sending 'Hi CPU' in 10\n", cdes_simulation_get_time(this->sim), (unsigned long)pthread_self());
    msg->from = this;
    strcpy(msg->data, "Hi CPU\0");
    cdes_simulation_event_notify(this->sim, this->out_event, 10, msg);
    return NULL;
}

simple_memory* simple_memory_create(simple_memory* this, cdes_simulation* sim) {
    cdes_task_create(&this->on_receive_msg, this, simple_memory_on_receive_msg, 0);
    cdes_event_create(&this->in_event);
    this->sim = sim;
    cdes_event_register_task(&this->in_event, &this->on_receive_msg);
    return this;
}

void simple_memory_destroy(simple_memory* this) {
    cdes_task_destroy(&this->on_receive_msg);
    cdes_event_destroy(&this->in_event);
}