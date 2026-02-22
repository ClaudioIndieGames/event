#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "simple_memory.h"

static void* on_receive_msg(void* module, void* args) {
    simple_memory* this = module;
    simple_port_message* msg = args;

    printf("[%s, %ld, Memory] Received '%s' from %p\n",
        *cdes_time_to_string(cdes_simulation_get_time(this->sim), &(cdes_time_string){}),
        (unsigned long)pthread_self(),
        msg->data,
        msg->from);
    
    cdes_time delay = 10e-3;
    msg->from = this;
    strcpy(msg->data, "Hi CPU\0");
    simple_port_send(&this->p, msg, delay);

    printf("[%s, %ld, Memory] Sending '%s' in %s\n",
        *cdes_time_to_string(cdes_simulation_get_time(this->sim), &(cdes_time_string){}),
        (unsigned long)pthread_self(),
        msg->data,
        *cdes_time_to_string(delay, &(cdes_time_string){}));

    return NULL;
}

simple_memory* simple_memory_create(simple_memory* this, cdes_simulation* sim) {
    cdes_task_create(&this->on_received_message, this, on_receive_msg, 0);
    simple_port_create(&this->p, sim, &this->on_received_message);
    this->sim = sim;
    return this;
}

void simple_memory_destroy(simple_memory* this) {
    cdes_task_destroy(&this->on_received_message);
    simple_port_destroy(&this->p);
}