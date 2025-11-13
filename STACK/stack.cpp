#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "stack.h"
#include "stack_define.h"
#include "config.h"
#include "color.h"





stack_error_t stack_creator(stack_struct* stack_address,
                            const long int start_capacity,
                            const char* file_name,
                            const int line,
                            const char* stack_name) {
    // assert(stack_address);
    assert(file_name);
    assert(stack_name);

//------------------------------
#if MOD_WORK == PROCESSOR
    (void) line;
    (void) stack_name;
    (void) file_name;
#endif
//-------------------------------

    if (file_name == NULL) {
        printf("FILE NULL ADDRESS: Name: %s\n", stack_name);
        return MUST_STOP;
    }

    if (MOD_WORK == DEBUG && start_capacity == 0) {
        printf("%sWARNING!!! Start capacity = 0%s\n", _P_, _N_);
    }

    stack_address->capacity = start_capacity;
    stack_address->size = 0;


    STACK_STR_INF(stack_address);
    STACK_STR_CAPACITY_CHECK(stack_address, start_capacity);

                                            // Мы точно знаем, что это значение > 0
    stmn_t* buffer_address = (stmn_t*) calloc(((size_t) start_capacity + 2 * SIZE_ADD_CAPACITY), sizeof(stmn_t));

    STACK_STR_ADDRESS_CHECK(stack_address, buffer_address);
    stack_address->data = buffer_address + SIZE_ADD_CAPACITY;

//------------------------------------------------------------------
#if COMPLETION_DATA == ON
    for (long int i = SIZE_ADD_CAPACITY; i <= start_capacity; i++) {
        buffer_address[i] = POISON_NUM;
    }

    if (SIZE_ADD_CAPACITY > 0) {
        buffer_address[0] = BIRD_NUM;
        buffer_address[start_capacity + SIZE_ADD_CAPACITY] = BIRD_NUM;
    }
#endif // COMPLETION_DATA
//------------------------------------------------------------------

//--------------------------------
#if HASH_SECURE == ON
REPLACE_HASH_CODE(stack_address);
#endif // HASH_SECURE == 1
//----------------------------------

    return NOT_ERRORS;
}


int stack_destruct(stack_struct* stack_address) {
    assert(stack_address);

    free(stack_address->data - SIZE_ADD_CAPACITY);
    return 0;
}


// ----------------------------------------------------------------------------------------------------
stmn_t stack_get_value (stack_struct* stack, int index)
{
    assert (stack);

    if (stack->size <= index)
    {
        printf ("ERROR: too big index to get value\n");
        return NULL;
    }

    return stack->data[index];
}
// ----------------------------------------------------------------------------------------------------


stack_error_t stack_push(stack_struct* stack_address, const stmn_t mean_to_push) {
    assert(stack_address);

    DUMP_NOT_CORRECT_STACK(stack_address);
    STACK_PUSH_CHECK(stack_address);
    STACK_PUSH_NUM_CHECK(stack_address, mean_to_push);

    stack_address->data[stack_address->size++] = mean_to_push;

//---------------------------------
#if HASH_SECURE == ON
REPLACE_HASH_CODE(stack_address);
#endif
//-----------------------------------
    DUMP_NOT_CORRECT_STACK(stack_address);

    return NOT_ERRORS;
}


stack_error_t stack_pop(stack_struct* stack_address, stmn_t* mean_pop_address) {
    assert(stack_address);
    assert(mean_pop_address);

    DUMP_NOT_CORRECT_STACK(stack_address);
    STACK_POP_CHECK(stack_address);


    *mean_pop_address = stack_address->data[--(stack_address->size)];

//--------------------------------
#if HASH_SECURE == 1
REPLACE_HASH_CODE(stack_address);
#endif
//---------------------------------
    DUMP_NOT_CORRECT_STACK(stack_address);

    return NOT_ERRORS;
}


//-----------------------------------------------------------------------------------------
// Функция stack_error при разных MOD_WORK
// Если DEBUG - то выполняем все проверки
#if MOD_WORK == DEBUG
int stack_error(stack_struct* stack_address) {
    // Нужно возвращать int для передачи не одной ошибки, а нескольких

    assert(stack_address);

    int return_error = 0;

    if (stack_address->data < (stmn_t*) MIN_ADDRESS && stack_address->capacity != 0) {
        return_error |= BAD_DATA_ADDRESS; // 1
    }

    if (stack_address->size < 0) {
        return_error |= BAD_SIZE; // 2
    }

    if (stack_address->capacity < 1 &&
    (return_error & BAD_DATA_ADDRESS) != BAD_DATA_ADDRESS) {
        return_error |= BAD_CAPACITY; // 4
    }

    if (stack_address->size > 0 &&
    stack_address->data[stack_address->size - 1] == POISON_NUM) {
        return_error |= BAD_CURRENT_ELEMENT; // 8
    }

    if (stack_address->size > stack_address->capacity) {
        return_error |= SIZE_BIGGER_CAPACITY; // 16
    }

    if (SIZE_ADD_CAPACITY != 0 && (stack_address->data[-1] != BIRD_NUM ||
        stack_address->data[stack_address->capacity] != BIRD_NUM)) {
        return_error |= BIRD_NOT_CORRECT; // 4096
    }

//-----------------------------------------
#if HASH_SECURE == ON
CHECK_HASH_CODE(stack_address, return_error);
#endif
//-------------------------------------------

    // printf("code error = %d\n", return_error);
    stack_address->inf_adr_error.current_error = return_error;
    return return_error;
}

//Если processor - то вообще не должна вызываться
#else
int stack_error(stack_struct* stack_address) {
    printf("ERROR with stack_error in USER_MODE\n");
    printf("But input: [%p]\n", stack_address);
    return 0;
}
#endif // MOD_WORK == DEBUG
//----------------------------------------------------------------------------------------


stack_error_t stack_realloc(stack_struct* stack_address) {
    assert(stack_address);

    DUMP_NOT_CORRECT_STACK(stack_address);

    stmn_t* buffer_address = (stmn_t*) realloc(stack_address->data - SIZE_ADD_CAPACITY,
                    ((size_t) (stack_address->capacity * MOD_REALLOC + 2 * SIZE_ADD_CAPACITY) )* sizeof(stmn_t));

    STACK_REALLOC_ADDRESS(stack_address, buffer_address);

    stack_address->capacity *= MOD_REALLOC;
    stack_address->data = buffer_address + SIZE_ADD_CAPACITY;

//-------------------------------------------------------------------
#if COMPLETION_DATA == ON
    for (ssize_t i = stack_address->size + 1; i <= stack_address->capacity; i++) {
        buffer_address[i] = POISON_NUM;
    }

    if (SIZE_ADD_CAPACITY > 0) {
        buffer_address[0] = BIRD_NUM;
        buffer_address[stack_address->capacity + SIZE_ADD_CAPACITY] = BIRD_NUM;
    }
#endif // COMPLETION_DATA
//------------------------------------------------------------------------

    return NOT_ERRORS;
}


//--------------------------------------------------------------------------------------------------------
// dump и принты для него
// Опять же, если DEBUG - то stack_dump - необходим
#if MOD_WORK == DEBUG
int stack_dump(stack_struct* stack_address) {
    assert(stack_address);

    int error_with_stack = stack_address->inf_adr_error.current_error;

    printf("\n%s===============================================================================\n", _R_);
    printf("--- STACK DUMP ---%s\n", _N_);
    printf("%sstack %s<%s> [%p]%s from %s%s:%d %sname stack:%s %s%s \nERRORS: \n",
            _P_,   _B_, name_type, stack_address, _R_, _B_,
            stack_address->inf_adr_location.creation_file,
            stack_address->inf_adr_location.creation_line, _P_, _B_,
            stack_address->inf_adr_location.name_stack, _R_);

    print_errors_for_dump(error_with_stack);
    printf("In Function: %s:%d\n\n", stack_address->inf_adr_error.error_file,
                                  stack_address->inf_adr_error.error_line);
    description_error(error_with_stack);
    // printf("ERROR_WITH_STACK = %d\n", error_with_stack);

    if ((error_with_stack & BAD_CREATE_CALLOC) == BAD_CREATE_CALLOC ||
        (error_with_stack & BAD_DATA_ADDRESS) == (BAD_DATA_ADDRESS) ||
        (error_with_stack & BAD_REALLOC) == BAD_REALLOC /*||
        stack_address->size < 1*/) {
        printf("%s===============================================================================\n%s",_P_, _N_);
        return 0;
    }

    printf("%s{\n", _P_);

    printf("size = %s%zd%s\n", _B_, stack_address->size, _P_);
    printf("capacity = %s%zd%s\n", _B_, stack_address->capacity, _P_);
    printf("data %s[%p]%s\n", _B_, stack_address->data, _P_);

//------------------------------------------------------------------------------
#if HASH_SECURE == 1

printf("\n");
printf("inital hash = %s%zd%s\n", _B_, stack_address->inf_hash_code.hash_code, _P_);
printf("modified hash = %s%zd%s\n", _B_, make_hash_code(stack_address), _P_);

#endif
//------------------------------------------------------------------------------

    printf("    {\n");

    print_stack_for_dump(stack_address, error_with_stack);

    printf("    }\n");
    printf("}\n");
    printf("%s===============================================================================\n%s",_P_, _N_);
    return 0;
}


void description_error(const int error_with_stack) {
    if ((error_with_stack & BAD_DATA_ADDRESS) == BAD_DATA_ADDRESS) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[0]);
        printf("Data Address < %d\n%s\n", MIN_ADDRESS, _N_);
    }

    if ((error_with_stack & BAD_SIZE) == BAD_SIZE) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[1]);
        printf("Size In Stack < 0\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_CAPACITY) == BAD_CAPACITY) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[2]);
        printf("Capacity < 0\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_CURRENT_ELEMENT) == BAD_CURRENT_ELEMENT) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[3]);
        printf("Current Element Is Poison (");
        PRINT_ELEMENT(POISON_NUM);
        printf(")\n%s\n", _N_);
    }

    if ((error_with_stack & SIZE_BIGGER_CAPACITY) == SIZE_BIGGER_CAPACITY) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[4]);
        printf("Current Size Is Bigger Then Capacity\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_PUSH_SIZE) == BAD_PUSH_SIZE) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[5]);
        printf("In Mode NOT_AUTO_REALLOC\n");
        printf("Size == Capacity \n%s\n", _N_);
    }

    if ((error_with_stack & BAD_PUSH_MEAN) == BAD_PUSH_MEAN) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[6]);
        printf("Push Element == Poison (");
        PRINT_ELEMENT(POISON_NUM);
        printf(")\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_POP_SIZE) == BAD_POP_SIZE) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[7]);
        printf("Call Pop When Size == 0\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_REALLOC) == BAD_REALLOC) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[8]);
        printf("Realloc return NULL\n\n" _N_);
    }

    if ((error_with_stack & LOSE_MEANS) == LOSE_MEANS) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[9]);
        printf("New Capacity Of Stack < Current Size\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_CREATE_CALLOC) == BAD_CREATE_CALLOC) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[10]);
        printf("Calloc Return Null\n%s\n", _N_);
    }

    // if ((error_with_stack & PUSH_MEAN_WITHOUT_LIMIT) == PUSH_MEAN_WITHOUT_LIMIT) {
    //     printf(_R_ "-- %s ---\n", ALL_ERRORS[11]);
    //     printf("Push Mean > %d or < %d\n%s\n", MAX_MEAN, MIN_MEAN, _N_);
    // }

    if ((error_with_stack & BIRD_NOT_CORRECT) == BIRD_NOT_CORRECT) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[12]);
        printf("Bird Num Was Changed\n%s\n", _N_);
    }

    if ((error_with_stack & BAD_HASH) == BAD_HASH) {
        printf(_R_ "-- %s ---\n", ALL_ERRORS[13]);
        printf("Not Coincidence Hash Code\n%s\n", _N_);
    }
}


void print_errors_for_dump(const int error_with_stack){
    int buffer = 0;
    for (int i = 1; i < MUST_STOP; i <<= 1) {
        if ((error_with_stack & i) == i) {
            printf("     %s (%d) \n", ALL_ERRORS[buffer], i);
        }
        buffer++;
    }
}


void print_stack_for_dump(stack_struct* stack_address, const int error_with_stack) {
    assert(stack_address);

    if (SIZE_ADD_CAPACITY > 0) {
        printf(_M_ "    first bird = ");
        printf("%p", (stack_address->data)[0 - SIZE_ADD_CAPACITY]);
        // PRINT_ELEMENT((stack_address->data)[0 - SIZE_ADD_CAPACITY]);
        printf(" (BIRD_NUM) (MUST BE ");
        printf("%p",  BIRD_NUM);
        printf(")\n" _N_);
    }
/*MIN_INT(stack_address->size + SIZE_ADD_CAPACITY * 2, AMOUNT_PRINT_ELEMENT)*/
    for (int i = 0; i < (stack_address->capacity); i++) {
        printf("   ");

        if ((error_with_stack & BAD_SIZE) != BAD_SIZE && i < stack_address->size) {
            printf("*");
        }
        else {
            printf(" ");
        }
        printf(_P_ "%d%s = " _B_, i, _N_);
        PRINT_ELEMENT((stack_address->data)[i]);
        printf("%s\n" _N_, _P_);
    }
    if (SIZE_ADD_CAPACITY > 0) {
        printf(_M_ "    second bird = ");
        printf("%p", (stack_address->data)[stack_address->capacity]);
        printf(" (BIRD_NUM) (MUST BE ");
        printf("%p", BIRD_NUM);
        printf(")\n" _N_);
    }
}


// В processor - не должны вызываться
#else
int stack_dump(stack_struct* stack_address) {
    printf("ERROR with dump in USER_MOD\n");
    printf("But input: [%p]\n", stack_address);
    return 0;
}


void print_errors_for_dump(const int error_with_stack) {
    printf("ERROR with print_errors_for_dump in USER_MODE\n");
    printf("But input: %d\n", error_with_stack);
}


void print_stack_for_dump(stack_struct* stack_address, const int error_with_stack) {
    printf("ERROR with print_stack_for_dump in USER_MODE\n");
    printf("But input: [%p] %d\n", stack_address, error_with_stack);
}

#endif //MOS_START == DEBUG
//-------------------------------------------------------------------------------------------------

ssize_t make_hash_code(stack_struct* stack_address) {
    assert(stack_address);

    // if (stack_address->capacity < 2 * SIZE_ADD_CAPACITY) {
    //     return 0;
    // }

    ssize_t current_hash = START_HASH_CODE;

    for (ssize_t i = 0; i < stack_address->capacity - 1; i++) {
        current_hash += current_hash * 33 + (ssize_t) stack_address->data[i];
    }

    return current_hash;
}




