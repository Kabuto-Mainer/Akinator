#ifndef STACK_H
#define STACK_H

#include "stack_define.h"
#include "config.h"

const long int SIZE_ADD_CAPACITY = BIRD_SECURE;


enum stack_error_t{
    NOT_ERRORS = 0,
    BAD_DATA_ADDRESS = 1,
    BAD_SIZE = 2,
    BAD_CAPACITY = 4,
    BAD_CURRENT_ELEMENT = 8,
    SIZE_BIGGER_CAPACITY = 16,
    BAD_PUSH_SIZE = 32,
    BAD_PUSH_MEAN = 64,
    BAD_POP_SIZE = 128,
    BAD_REALLOC = 256,
    LOSE_MEANS = 512,
    BAD_CREATE_CALLOC = 1024,
    PUSH_MEAN_WITHOUT_LIMIT = 2048,
    BIRD_NOT_CORRECT = 4096,
    BAD_HASH = 8192,
    MUST_STOP = 16384
};


const char ALL_ERRORS[20][40] = {"BAD_DATA_ADDRESS",
                                "BAD_SIZE",
                                "BAD_CAPACITY",
                                "BAD_CURRENT_ELEMENT",
                                "SIZE_BIGGER_CAPACITY",
                                "BAD_PUSH_SIZE",
                                "BAD_PUSH_MEAN",
                                "BAD_POP_SIZE",
                                "BAD_REALLOC",
                                "LOSE_MEANS",
                                "BAD_CREATE_CALLOC",
                                "PUSH_MEAN_WITHOUT_LIMIT",
                                "BIRD_NOT_CORRECT",
                                "BAD_HASH",
                                "MUST_STOP"
};


ssize_t make_hash_code(stack_struct* stack_address);

int stack_error(stack_struct* stack_address);

int stack_dump(stack_struct* stack_address);



void print_errors_for_dump(const int error_with_stack);

void description_error(const int error_with_stack);

void print_stack_for_dump(stack_struct* stack_address,
                          const int error_with_stack);




stack_error_t stack_pop(stack_struct* stack_address,
                        stmn_t* mean_pop_address);

stack_error_t stack_push(stack_struct* stack_address,
                         const stmn_t mean_to_push);

stmn_t stack_get_value (stack_struct* stack,
                        int index);



int stack_destruct(stack_struct* stack_address);


stack_error_t stack_creator(stack_struct* stack_address,
                        const long int start_capacity,
                        const char* file_name,
                        const int line_number,
                        const char* stack_name);


stack_error_t stack_realloc(stack_struct* stack_address);




#endif // STACK_H


