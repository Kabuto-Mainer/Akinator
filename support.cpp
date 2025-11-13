#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "config.h"
// #include "const.h"
#include "type.h"
#include "func.h"




// --------------------------------------------------------------------------------------------------
int get_hash (const char* string)
{
    assert (string);

    int index = 0;
    int hash = 5381;
    while (string[index] != '\0')
    {
        hash = int ((size_t) hash * 33 + (size_t) string[index]);
        index++;
    }

    return hash;
}
// --------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------
int print_delay_string (const char* string)
{
    assert (string);

    for (size_t i = 0; i < strlen (string); i++)
    {
        putchar (string[i]);
        msleep (PRINT_SLEEP_TIME);
        fflush(stdout);
    }
    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int my_print (const char* format, ...)
{
    assert (format);

    va_list args = {};
    va_start (args, format);

    while (*format != '\0')
    {
        if (*format == '%' && *(format + 1) != '\0')
        {
            format++;

            switch (*format)
            {
                case 's':
                {
                    const char* string = va_arg (args, const char*);
                    print_delay_string (string);
                    break;
                }

                case 'd':
                {
                    int number = va_arg (args, int);
                    char* string = int_to_char (number);
                    print_delay_string (string);
                    free (string);
                    break;
                }

                case '%':
                {
                    putchar ('%');
                    fflush(stdout);
                    msleep (PRINT_SLEEP_TIME);
                    break;
                }

                default:
                {
                    printf ("Sorry, supported only \"%%s\" species\n");
                }
            }
        }

        else
        {
            putchar (*format);
            fflush(stdout);
            msleep (PRINT_SLEEP_TIME);
        }
        format++;
    }

    va_end (args);
    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
void msleep (long int milisec)
{
    struct timespec time = {};
    time.tv_sec = milisec / 1000;
    time.tv_nsec = (milisec % 1000) * 1000000;

    nanosleep(&time, NULL);
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
char* int_to_char (int value)
{
    int i = 1;
    unsigned int len = 1;

    while (value % i != 0)
    {
        i *= 10;
        len++;
    }

    char* buffer = (char*) calloc (len + 1, sizeof (char));
    if (buffer == NULL)
    {
        EXIT_FUNC("NULL calloc", NULL);
    }

    unsigned int count = 0;
    for (i = 1; count < len; i *= 10, count++)
    {
        buffer[count] = char ('0' + (value % (i * 10)) / i);
    }

    buffer[len] = '\0';

    return buffer;
}
// --------------------------------------------------------------------------------------------------




// -------------------------------------------------------------------------------------------------------
size_t file_size_check(const char* name_file)
{
    assert(name_file);

    struct stat file_stat = {};

    if (stat(name_file, &file_stat) == -1)
    {
        printf("ERROR with stat\n");
        return 0;
    }
//     Вроде как размер не должен быть отрицательным
    return (size_t) file_stat.st_size;
}
// -------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------
size_t get_size_object (obj_t object)
{
    ASSERT_OBJECT(object);

    size_t counter = 0;
    while (1)
    {
        if (object.name[counter] == '\0')
        {
            return counter;
        }
        counter++;
    }
    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int find_number_dump ()
{
    return STANDARD_DIR_DUMP;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int make_dir_address (const char* address_dir,
                      const int number_dir,
                      const char* address_file,
                      char* string)
{
    assert (address_dir);
    assert (string);

    sprintf (string, "%s%d/%s", address_dir, number_dir, address_file);
    return 0;
}
// --------------------------------------------------------------------------------------------------



// -------------------------------------------------------------------------------------------------------
int clean_html (const char* address_html)
{
    assert (address_html);

    if (access (address_html, F_OK) != 0)
    {
        printf ("File does not exist\n");
        return -1;
    }

    FILE* file = fopen (address_html, "w");
    if (file == NULL)
    {
        EXIT_FUNC ("NULL file\n", -1);
    }

    fprintf (file,
             "<pre style=\"font-family: 'Courier New', monospace;"
             "font-size: 14px; color: #e0e0e0; background-color: #1e1e1e; padding: 10px; border-radius: 6px;\">\n\n");
    fclose (file);
    return 0;

}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int clean_dir (const char* address_dir)
{
    assert (address_dir);

    char comand[200] = "";
    sprintf (comand, "rm -rf ./%s/*", address_dir);
    int trash = system (comand);
    (void) trash;

    return 1;
}
// -------------------------------------------------------------------------------------------------------







