#ifndef CONFIG_STACK_H
#define CONFIG_STACK_H

#include "../H/type.h"

#define OFF 0
#define ON 1

#define PROCESSOR 0
#define DEBUG 1


#define MOD_WORK DEBUG

// Канареечная зашита
#define BIRD_SECURE OFF

// Хеш зашиты
#define HASH_SECURE OFF

//todo config.h
// Автоматическое увеличение стека
#define AUTO_REALLOC ON

// Заполнение стека
//* Необходимо для работы Канареечной и Хеш зашиты
#define COMPLETION_DATA OFF


// Тип данных стека. В режиме процессора должен быть <int>
typedef node_t* stmn_t;
const char name_type[] = "node_t*";
#define PRINT_ELEMENT(num) { \
   printf("%p", num); \
}

// Множитель realloc
const int MOD_REALLOC = 2;
const int REALLOC_CONST = 5;

const int START_SIZE_STACK = 4;

// Сид для хеш-функции
const size_t START_HASH_CODE = 5318;

// Ядовитое значение для стека
const stmn_t POISON_NUM = NULL;

// Минимальные рабочий адрес
const int MIN_ADDRESS = 8000;

// Количество элементов, которое выведется, если size > этого значения
const int AMOUNT_PRINT_ELEMENT = 10;

// Куропатка или Канарейка
const stmn_t BIRD_NUM = NULL;

#endif // CONFIG_STACK_H
