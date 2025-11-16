#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// extern button_desc_t;

// ----------------------------------------------------------------------------------------------------
// Параметры графики
// ----------------------------------------------------------------------------------------------------
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
// ----------------------------------------------------------------------------------------------------
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 45;
// ----------------------------------------------------------------------------------------------------
const int AMOUNT_BUTTONS = 6;
// ----------------------------------------------------------------------------------------------------
const SDL_Color COLOR_FONT = {255, 255, 255, 255};
// ----------------------------------------------------------------------------------------------------
const char DISPLAY_FONT[] = "GRAPHIC/SYSTEM/font.ttf";
#define BACKGROUND_ADR  "GRAPHIC/SYSTEM/BACK/%d.png"
#define SIDE_VIDEO_ADR "GRAPHIC/SYSTEM/SIDE/%d.png"
#define MAIN_VIDEO_ADR "GRAPHIC/SYSTEM/MAIN/%d.jpg"
#define USER_VIDEO_ADR "GRAPHIC/SYSTEM/USER/%d.jpg"
#define ANIM_VIDEO_ADR "GRAPHIC/SYSTEM/ANIM/%d/%d.png"
// ----------------------------------------------------------------------------------------------------
const int LEN_VIDEO_BACKGROUND = 20;
const int LEN_VIDEO_MAIN = 1;
const int LEN_VIDEO_USER = 1;
// ----------------------------------------------------------------------------------------------------
const int AMOUNT_ANIM = 1;
const int LEN_VIDEO_ANIM = 8;
// ----------------------------------------------------------------------------------------------------
const int SIZE_LINE = 2;
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
const int CENTER_MAIN_TEXT_X = SCREEN_WIDTH / 2;
const int CENTER_MAIN_TEXT_Y = 200 / 2;
const int CENTER_USER_TEXT_X = SCREEN_WIDTH / 2;
const int CENTER_USER_TEXT_Y = 750;
// ----------------------------------------------------------------------------------------------------
const int MAX_USER_LEN_TEXT = 40;
const Uint64 TIME_DELAY_CURSOR = 500;
// ----------------------------------------------------------------------------------------------------



#endif // DISPLAY_CONFIG_H
