#ifndef TYPE_H
#define TYPE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "config.h"
#include "display-config.h"


// ---------------------------------------------------------------------------------------------------
enum PLACE_BUTTON
{
    LEFT_DOWN   = 0,
    RIGHT_DOWN  = 1,
    LEFT_MID    = 2,
    RIGHT_MID   = 3,
    LEFT_UP     = 4,
    RIGHT_UP    = 5
};
// --------------------------------------------------------------------------------------------------
enum SIDES
{
    LEFT_SIDE   = 0,
    RIGHT_SIDE  = 1
};
// --------------------------------------------------------------------------------------------------
enum BOOL
{
    USER_YES = 0,
    USER_NO  = 1
};
// --------------------------------------------------------------------------------------------------
enum EVENT
{
    NULL_EVENT          = 0,
    START_PROGRAM       = 1,
    GUESS_OBJECT        = 2,
    COMPARE_OBJECTS     = 3,
    DESC_OBJECT         = 4,
    SAVE_DATA           = 5,
    IMPORT_DATA         = 6,
    EXIT_PROGRAM        = 7
};
// --------------------------------------------------------------------------------------------------
enum TYPE_OBJECT
{
    NO_OWN_MEMORY   = 0x0000,
    OWN_MEMORY      = 0x0001,
    IS_AUDIO        = 0x0002,
    IS_IMAGE        = 0x0004,
};
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
struct button_t
{
    SDL_Texture* but;
    PLACE_BUTTON type;
};
// --------------------------------------------------------------------------------------------------
struct button_desc_t
{
    const char* name;
    PLACE_BUTTON place;
    EVENT event;
};
// --------------------------------------------------------------------------------------------------
struct frame_t
{
    char* main_text;
    char* user_text;
    char* picture;
    char* audio;
    button_t buttons[AMOUNT_BUTTONS];
    int amount_but;
};
// --------------------------------------------------------------------------------------------------
struct display_t
{
    SDL_Window* window;
    SDL_Renderer* render;
    TTF_Font* font;
    SDL_Texture* background;
    frame_t cur_frame;
};
// --------------------------------------------------------------------------------------------------
struct object_t
{
    char* name;
    int hash;
    char* audio;
    char* image;
    int type_object;
};
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
typedef object_t obj_t;
// #define BAD_OBJECT
// --------------------------------------------------------------------------------------------------
struct node_t
{
    node_t* father;
    node_t* left;
    node_t* right;
    obj_t object;
};
// --------------------------------------------------------------------------------------------------
struct tree_t
{
    node_t* null;
    size_t size;
    int num_dump;
    char* buffer;
    display_t* display;
};
// --------------------------------------------------------------------------------------------------




#endif // TYPE_H
