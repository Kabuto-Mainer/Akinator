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
    RIGHT_UP    = 5,
    CENTER_UP   = 6
};
// --------------------------------------------------------------------------------------------------
enum __ANIM_TYPE
{
    STANDARD_ANIM       = 10, // Отсутствие анимации
    TYPING_ANIM         = 0,
    PLAY_AUDIO_ANIM     = 1,
    RECORD_AUDIO_ANIM   = 2
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
struct _video_t
{
    SDL_Texture** frames;
    int amount;
    int current;
    const SDL_Rect* place;

    int delay;
    int time_left;
};
// --------------------------------------------------------------------------------------------------
struct _audio_t
{
    SDL_AudioDeviceID record;
    SDL_AudioSpec param_record;
    SDL_AudioDeviceID play;
    SDL_AudioSpec param_play;
};
// --------------------------------------------------------------------------------------------------
struct __WAV_head {
    char riff[4];           // Заголовок (формат "RIFF")
    uint32_t chunk_size;    // Размер файла
    char wave[4];           // Тип ("WAVE")
    char format[4];         // Какой идет формат
    uint32_t subchunk1Size; // Длина подблока
    uint16_t audio_format;  // Формат аудио
    uint16_t num_channels;  // Количество каналов
    uint32_t sample_rate;   // Частота дискретизации
    uint32_t byte_rate;     // Количество байтов, которые нужно воспроизводить, в секунду
    uint16_t block_align;   // Количество байтов на семпл
    uint16_t bitsPerSample; // Битность семпла
    char data[4];           // Начало данных ("data")
    uint32_t data_size;     // Размер аудиоданных в байтах
};
// --------------------------------------------------------------------------------------------------
struct _frame_t
{
    char* main_text;
    char* user_text;

    char* obj_img;
    char* audio;

    button_t buttons[AMOUNT_BUTTONS];
    int amount_but;
    __ANIM_TYPE anim_type;
};
// --------------------------------------------------------------------------------------------------
struct _system_interface_t
{
    _video_t background;
    _video_t main;
    _video_t user;
    _video_t* mass_anim;
};
// --------------------------------------------------------------------------------------------------
struct display_t
{
    SDL_Window* window;
    SDL_Renderer* render;
    TTF_Font* font;
    _system_interface_t system;
    _frame_t cur_frame;
    _audio_t audio_data;
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

extern const button_desc_t MAIN_MENU_BUN[];


#endif // TYPE_H
