#include <stdlib.h>
#include <assert.h>
#include "simple_message.h"

simple_message* simple_message_create(size_t size) {
    simple_message* this = malloc(sizeof(simple_message));
    assert(this && "Malloc failed");
    this->data = malloc(sizeof(char) * size);
    assert(this->data && "Malloc failed");
    return this;
}

void simple_message_destroy(simple_message* this) {
    assert(this && "Passed NULL message");
    assert(this->data && "Passed NULL data");
    free(this->data);
    free(this);
}