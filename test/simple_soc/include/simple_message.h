#ifndef __SIMPLE_MESSAGE_H__
#define __SIMPLE_MESSAGE_H__

#include <stddef.h>

typedef struct {
    void* from;
    char* data;
} simple_message;

simple_message* simple_message_create(size_t size);
void simple_message_destroy(simple_message* this);

#endif  // __SIMPLE_MESSAGE_H__