#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <assert.h>
#include <unistd.h>

#include "../H/type.h"
#include "../H/config.h"
#include "../H/const.h"


// --------------------------------------------------------------------------------------------------
int create_button (button_t* button,
                   SDL_Renderer* render,
                   const char* text,
                   TTF_Font* font);
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int create_display (display_t* display);
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int filling_button (display_t* display);
// --------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------
int create_display (display_t* display)
{
    assert (display);

    SDL_Init (SDL_INIT_VIDEO);
    TTF_Init ();

    SDL_Window* window = SDL_CreateWindow ("Button Example",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           SCREEN_WIDTH, SCREEN_HEIGHT,
                                           0);
    SDL_Renderer* renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont (DISPLAY_FONT, 24);
    if (font == NULL) {
        SDL_Log ("TTF_OpenFont failed: %s", TTF_GetError());
        return 1;
    }

    display->font = font;
    display->render = renderer;
    display->window = window;
    display->background = NULL;

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int filling_button (display_t* display)
{
    assert (display);

    for (int i = 0; i < AMOUNT_BUTTONS; i++)
    {
        create_button (&(display->buttons[i]),
                       display->render,
                       BUTTON_DESC_CHAR[i],
                       display->font);

        display->buttons[i].type = (TYPE_BUTTON) i;
    }

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int create_button(button_t* button,
                  SDL_Renderer* render,
                  const char* text,
                  TTF_Font* font)
{
    assert(button);
    assert(text);

    SDL_Texture* combined = SDL_CreateTexture (
        render,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    );

    SDL_SetTextureBlendMode (combined, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget (render, combined);

    SDL_SetRenderDrawColor (render, 0, 0, 0, 0);
    SDL_RenderClear (render);

    SDL_Rect but_rect = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_SetRenderDrawColor (render, 255, 120, 50, 255);
    SDL_RenderFillRect (render, &but_rect);

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended (font, text, color);

    SDL_Texture* text_tex = SDL_CreateTextureFromSurface (render, surface);
    SDL_SetTextureBlendMode (text_tex, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface (surface);

    int width = 0, height = 0;
    TTF_SizeUTF8 (font, text, &width, &height);

    SDL_Rect text_rect = {
        (BUTTON_WIDTH - width) / 2,
        (BUTTON_HEIGHT - height) / 2,
        width,
        height
    };

    SDL_RenderCopy (render, text_tex, NULL, &text_rect);
    SDL_DestroyTexture (text_tex);

    SDL_SetRenderTarget (render, NULL);
    button->but = combined;

    return 0;
}
// --------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------
int my_print (display_t* display,
              const char* format, ...)
{
    assert (format);

    va_list args = {};
    va_start (args, format);

    const char string_buffer[1024] = "";
    int index = 0;

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
                    strcpy (string_buffer[index], string);
                    index += strlen (string) + 1;
                    // print_delay_string (string);
                    break;
                }

                case 'd':
                {
                    int number = va_arg (args, int);
                    char* string = int_to_char (number);
                    strcpy (string_buffer[index], string);
                    index += strlen (string) + 1;
                    // print_delay_string (string);
                    free (string);
                    break;
                }

                case '%':
                {
                    string_buffer[index++] = '%';
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
            string_buffer[index++] = *format;
        }
        format++;
    }
    va_end (args);

    render_text (display, string);

    return 0;
}
// --------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------
int render_text (display_t* display,
                 const char* string)
{
    assert (display);
    assert (string);

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended (display->font, string, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface (display->render, surface);

    int width = 0;
    int height = 0;
    TTF_SizeUTF8(display->font, string, &width, &height);

    SDL_Rect size_tex = {CENTER_TEXT_X - width / 2,
                         CENTER_TEXT_Y - height / 2,
                         width,
                         height};

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &size_tex);
    SDL_DestroyTexture(texture);

    retrun 0;
}
// --------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция получения следующей задачи от пользователя
 * @param [in] prev Предыдущая задача
*/
// --------------------------------------------------------------------------------------------------
EVENT get_event (EVENT prev,
                 display_t* display)
{
    if (prev == NULL_EVENT)
    {
        my_print ("------------------------\n");
        my_print ("Welcome to this program\n");
        my_print ("Do you want to start? (Y\\n)\n");
        my_print ("------------------------\n");

        if (get_user_bool () == USER_YES)
        {
            return START_PROGRAM;
        }

        return EXIT_PROGRAM;
    }

    else if (prev == EXIT_PROGRAM)
    {
        // printf ("Do you want to save new data base? (Y\\n)\n");

        //         if (get_user_bool () == USER_YES)
        // {
        //     return SAVE_DATA;
        // }

        return EXIT_PROGRAM;
    }

    else
    {
        my_print ("-------------------------\n");
        my_print ("What do you want to do?\n");
        my_print (" - Guess an object (g)\n");
        my_print (" - Compare the objects (c)\n");
        my_print (" - Describe an object (d)\n");
        my_print (" - Save data base (s)\n");
        my_print (" - Import data base (i)\n");
        my_print (" - Exit program (e)\n");
        my_print ("-------------------------\n");
    }
    int comand = '\0';

    while (1)
    {
        my_print ("> ");
        comand = getchar();

        while (getchar() != '\n')
        {
            continue;
        }

        switch (toupper (comand))
        {
            case ('G'):
            {
                return GUESS_OBJECT;
            }

            case ('C'):
            {
                return COMPARE_OBJECTS;
            }

            case ('D'):
            {
                return DESC_OBJECT;
            }

            case ('S'):
            {
                return SAVE_DATA;
            }

            case ('I'):
            {
                return IMPORT_DATA;
            }

            case ('E'):
            {
                return EXIT_PROGRAM;
            }

            default:
            {
                my_print ("Please, re-enter you answer (you can use only 'g', 'c', 'd', 's', 'e')\n");
            }
        }
    }
    return EXIT_PROGRAM;
}
// --------------------------------------------------------------------------------------------------







int main ()
{
    display_t display = {};
    create_display (&display);
    filling_button (&display);

    int running = 1;
    SDL_Event e = {};

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        SDL_SetRenderDrawColor (display.render, 30, 30, 30, 255); // фон экрана
        SDL_RenderClear (display.render);

        // Рисуем кнопку
        SDL_RenderCopy (display.render, display.buttons[0].but , NULL, &(PLACE_BUTTON[display.buttons[0].type]));
        SDL_RenderCopy (display.render, display.buttons[1].but , NULL, &(PLACE_BUTTON[display.buttons[1].type]));
        SDL_RenderCopy (display.render, display.buttons[2].but , NULL, &(PLACE_BUTTON[display.buttons[2].type]));
        SDL_RenderCopy (display.render, display.buttons[3].but , NULL, &(PLACE_BUTTON[display.buttons[3].type]));
        SDL_RenderCopy (display.render, display.buttons[4].but , NULL, &(PLACE_BUTTON[display.buttons[4].type]));
        SDL_RenderCopy (display.render, display.buttons[5].but , NULL, &(PLACE_BUTTON[display.buttons[5].type]));



        SDL_RenderPresent (display.render);
    }

    // SDL_DestroyTexture(my_button.but);
    TTF_CloseFont(display.font);
    SDL_DestroyRenderer(display.render);
    SDL_DestroyWindow(display.window);
    TTF_Quit();
    SDL_Quit();

    return 1;
}
