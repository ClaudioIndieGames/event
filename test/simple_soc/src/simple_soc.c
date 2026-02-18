#include "simple_soc.h"

simple_soc* simple_soc_create(simple_soc* this, cdes_simulation* sim) {
    simple_cpu_create(&this->cpu, sim);
    simple_memory_create(&this->mem, sim);
    this->sim = sim;

    // connect cpu and memory
    this->cpu.out_event = &this->mem.in_event;
    this->mem.out_event = &this->cpu.in_event;

    return this;
}

void simple_soc_destroy(simple_soc* this) {
    simple_cpu_destroy(&this->cpu);
    simple_memory_destroy(&this->mem);
}