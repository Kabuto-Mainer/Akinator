#ifndef CONST_H
#define CONST_H


// Start object
// --------------------------------------------------------------------------------------------------
char START_OBJECT_NAME[] = "Unknown who";
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
const char SYNTAX_VOID[] =
{
    ' ',
    '\n',
    '\r',
    '\0',
    ',',
    '.'
};
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
const char BUTTON_DESC_CHAR[][30] =
{
    "Yes",
    "No",
    "Guess objects",
    "Description object",
    "Compare objects",
    "Import data base",
    "Save data base",
    "Start program",
    "Exit program",
    "Own file",
    "Standard directory"
};
// --------------------------------------------------------------------------------------------------
const button_desc_t MAIN_MENU_BUT[] =
{
    {"Guess object",        LEFT_UP,    GUESS_OBJECT},
    {"Compare objects",     LEFT_MID,   COMPARE_OBJECTS},
    {"Description object",  LEFT_DOWN,  DESC_OBJECT},
    {"Save data base",      RIGHT_UP,   SAVE_DATA},
    {"Upload data base",    RIGHT_MID,  IMPORT_DATA},
    {"Exit",                RIGHT_DOWN, EXIT_PROGRAM}
};
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------



#endif // CONST_H
