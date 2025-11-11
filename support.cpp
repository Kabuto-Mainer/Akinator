#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "config.h"
// #include "const.h"
#include "type.h"
#include "func.h"


// --------------------------------------------------------------------------------------------------
size_t get_size_object (obj_t object)
{
    ASSERT_OBJECT(object);

    size_t counter = 0;
    while (1)
    {
        if (object[counter] == '\0')
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

// --------------------------------------------------------------------------------------------------
size_t get_hash (const char* string)
{
    assert (string);

    int index = 0;
    size_t hash = 5381;
    while (string[index] != '\0')
    {
        hash = ((hash << 5) + hash) + (size_t) (string[index++]); // toupper for more simple use for users
    }

    return hash;
}
// --------------------------------------------------------------------------------------------------




