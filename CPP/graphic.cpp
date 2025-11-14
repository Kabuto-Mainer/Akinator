#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "../H/type.h"
#include "../H/config.h"
#include "../H/const.h"
#include "../H/display.h"
#include "../H/display-config.h"
#include "../H/func.h"

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания дисплея
 * @param [in] display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int create_display (display_t* display)
{
    assert (display);

    // SDL_Init (SDL_INIT_VIDEO);
    TTF_Init ();

// Необходимость для того, что бы не показывалось ложных ошибок и утечке памяти
    // setenv ("DBUS_SESSION_BUS_ADDRESS", "/dev/null", 1);
    // setenv ("SDL_AUDIODRIVER", "dummy", 1);


    SDL_Window* window = SDL_CreateWindow ("Akinator",
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
/**
 * @brief Функция удаления структуры дисплея
 * @param [in] display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int destroy_display (display_t* display)
{
    assert (display);

    for (int i = 0; i < AMOUNT_BUTTONS; i++)
    {
        if (display->cur_frame.buttons[i].but != NULL)
        {
            SDL_DestroyTexture (display->cur_frame.buttons[i].but);
        }
    }

    if (display->cur_frame.audio != NULL)
    {
        free (display->cur_frame.audio);
    }

    if (display->cur_frame.main_text != NULL)
    {
        free (display->cur_frame.main_text);
    }

    if (display->cur_frame.picture != NULL)
    {
        free (display->cur_frame.picture);
    }

    if (display->cur_frame.user_text != NULL)
    {
        free (display->cur_frame.user_text);
    }

    TTF_CloseFont(display->font);
    SDL_DestroyRenderer(display->render);
    SDL_DestroyWindow(display->window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания текстуры кнопки
 * @param [in] button Указатель на структуру кнопки, куда будет записана текстура
 * @param [in] render Указатель на окно для рендера
 * @param [in] text Текст кнопки
 * @param [in] font Шрифт кнопки
 * @param [in] color Цвет фона кнопки
*/
// --------------------------------------------------------------------------------------------------
int create_button (button_t* button,
                   SDL_Renderer* render,
                   const char* text,
                   TTF_Font* font,
                   SDL_Color color)
{
    assert(button);
    assert(text);

    SDL_Texture* combined = SDL_CreateTexture (render,
                                               SDL_PIXELFORMAT_RGBA8888,
                                               SDL_TEXTUREACCESS_TARGET,
                                               BUTTON_WIDTH,
                                               BUTTON_HEIGHT);

    SDL_SetTextureBlendMode (combined, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget (render, combined);

    SDL_SetRenderDrawColor (render, 0, 0, 0, 0);
    SDL_RenderClear (render);

    SDL_Rect but_rect = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect (render, &but_rect);

    SDL_Color color_text = COLOR_FONT;
    SDL_Surface* surface = TTF_RenderUTF8_Blended (font, text, color_text);

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

    if (button->but != NULL)
    {
        SDL_DestroyTexture (button->but);
    }

    button->but = combined;

    return 0;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция добавления текста в основное окна
 * @param [in] display Указатель на структуру дисплея
 * @param [in] format Формат строки, которую нужно добавить
 * @note Читает только %s и %d
 * @note Удаляет предыдущий текст
*/
// --------------------------------------------------------------------------------------------------
int add_text (display_t* display,
              const char* format, ...)
{
    assert (format);

    va_list args = {};
    va_start (args, format);

    char string_buffer[1024] = "";
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
                    strcpy (string_buffer + index, string);
                    index += (int) strlen (string);
                    index++;
                    // print_delay_string (string);
                    break;
                }

                case 'd':
                {
                    int number = va_arg (args, int);
                    char* string = int_to_char (number);
                    strcpy (string_buffer + index, string);
                    index += (int) strlen (string) + 1;
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

    string_buffer[index] = '\0';

    if (display->cur_frame.main_text != NULL)
    {
        free (display->cur_frame.main_text);
    }

    display->cur_frame.main_text = strdup (string_buffer);

    return 0;
}
// --------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция обновления экрана
 * @param [in] display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int renew_display (display_t* display)
{
    assert (display);

    SDL_SetRenderDrawColor (display->render, 30, 30, 30, 255);
    SDL_RenderClear (display->render);

    render_main_text (display);
    render_buttons (display);
    render_user_text (display);
    SDL_RenderPresent (display->render);
    // render_animation (display);

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция вывода текущего текста основного окна
 * @param [in]  display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int render_main_text (display_t* display)
{
    assert (display);

    if (display->cur_frame.main_text == NULL)
    {
        return 0;
    }

    SDL_Color color = COLOR_FONT;
    SDL_Surface* surface = TTF_RenderUTF8_Blended (display->font, display->cur_frame.main_text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface (display->render, surface);

    int width = 0;
    int height = 0;
    TTF_SizeUTF8 (display->font, display->cur_frame.main_text, &width, &height);

    SDL_Rect size_tex = {CENTER_MAIN_TEXT_X - width / 2,
                         CENTER_MAIN_TEXT_Y - height / 2,
                         width,
                         height};

    SDL_FreeSurface (surface);

    SDL_RenderCopy (display->render, texture, NULL, &size_tex);
    SDL_DestroyTexture (texture);

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция обновления кнопок (по запеченным текстурам)
 * @param [in] display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int render_buttons (display_t* display)
{
    assert (display);

    for (int i = 0; i < display->cur_frame.amount_but; i++)
    {
        SDL_RenderCopy (display->render,
                        display->cur_frame.buttons[i].but,
                        NULL,
                        &(PLACE_BUTTON_MASS[display->cur_frame.buttons[i].type]));
    }

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция отрисовки пользовательского текста
 * @param [in] display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int render_user_text (display_t* display)
{
    assert (display);

    if (display->cur_frame.user_text == NULL)
    {
        return 0;
    }

    SDL_Color color = COLOR_FONT;
    SDL_Surface* surface = TTF_RenderUTF8_Blended (display->font, display->cur_frame.user_text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface (display->render, surface);

    int width = 0;
    int height = 0;
    TTF_SizeUTF8(display->font, display->cur_frame.user_text, &width, &height);

    SDL_Rect size_tex = {CENTER_USER_TEXT_X - width / 2,
                         CENTER_USER_TEXT_Y - height / 2,
                         width,
                         height};

    SDL_Rect line_tex = {CENTER_USER_TEXT_X - width / 2 - 5,
                         CENTER_USER_TEXT_Y + height / 2 + 1,
                         width + 10,
                         SIZE_LINE};

    SDL_SetRenderDrawColor (display->render, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect (display->render, &line_tex);

    SDL_FreeSurface (surface);

    SDL_RenderCopy (display->render, texture, NULL, &size_tex);
    SDL_DestroyTexture (texture);

    return 0;
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
        add_text (display,
                  "Welcome to this program\n"
                  "Do you want to start?");

        if (get_user_bool (display) == USER_YES)
        {
            return START_PROGRAM;
        }

        return EXIT_PROGRAM;
    }

    else if (prev == EXIT_PROGRAM)
    {
        return EXIT_PROGRAM;
    }

    add_text (display,
              "What do you want to do?");
    button_t* all_button[(sizeof (MAIN_MENU_BUT) / sizeof (MAIN_MENU_BUT[0]))] = {};

    for (size_t i = 0; i < (sizeof (MAIN_MENU_BUT) / sizeof (MAIN_MENU_BUT[0])); i++)
    {
        all_button[i] = &(display->cur_frame.buttons[i]);
        create_button (all_button[i],
                       display->render,
                       MAIN_MENU_BUT[i].name,
                       display->font,
                       BASE_BUTTON_COLOR);

        all_button[i]->type = MAIN_MENU_BUT[i].place;
    }

    display->cur_frame.amount_but = (sizeof (MAIN_MENU_BUT) / sizeof (MAIN_MENU_BUT[0]));
    display->cur_frame.audio = NULL;
    display->cur_frame.picture = NULL;
    display->cur_frame.user_text = NULL;

    int running = true;
    SDL_Event event = {};

    renew_display (display);
    while (running)
    {
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
            {
                return EXIT_PROGRAM;
            }

            for (size_t i = 0; i < (sizeof (MAIN_MENU_BUT) / sizeof (MAIN_MENU_BUT[0])); i++)
            {
                if (check_button_click (all_button[i], &event) == 1)
                {
                    create_button (all_button[i],
                                   display->render,
                                   MAIN_MENU_BUT[i].name,
                                   display->font,
                                   CLICK_BUTTON_COLOR);

                    renew_display (display);
                    return MAIN_MENU_BUT[i].event;
                }
            }
            renew_display (display);
        }
        SDL_Delay (40);
    }
    return EXIT_PROGRAM;
}
// --------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция получения ответа типа 'да/нет' от пользователя
 * @return USER_YES (если да), иначе -> USER_NO
 * @note Сама обновляет дисплей
*/
// ----------------------------------------------------------------------------------------------------
int get_user_bool (display_t* display)
{

    button_t* yes = &(display->cur_frame.buttons[0]);
    button_t* no = &(display->cur_frame.buttons[1]);

    create_button (yes,
                   display->render,
                   "YES",
                   display->font,
                   BASE_BUTTON_COLOR);
    create_button (no,
                   display->render,
                   "NO",
                   display->font,
                   BASE_BUTTON_COLOR);

    display->cur_frame.amount_but = 2;
    display->cur_frame.audio = NULL;
    display->cur_frame.picture = NULL;
    display->cur_frame.user_text = NULL;

    yes->type = LEFT_UP;
    no->type = RIGHT_UP;

    int running = 1;
    SDL_Event event = {};

    renew_display (display);
    while (running)
    {
        while (SDL_PollEvent (&event))
        {

            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            if (check_button_click (yes, &event) == 1)
            {
                create_button (yes,
                               display->render,
                               "YES",
                               display->font,
                               CLICK_BUTTON_COLOR);

                renew_display (display);
                return USER_YES;
            }

            else if (check_button_click (no, &event) == 1)
            {
                create_button (no,
                               display->render,
                               "NO",
                               display->font,
                               CLICK_BUTTON_COLOR);

                renew_display (display);
                return USER_NO;
            }
        }
    }

    return USER_YES;
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция считывания пользовательского текста
 * @param [in] display Указатель на структуру дисплея
 * @param [in] string Указатель на адрес, куда будет записан пользовательский текст
*/
// ----------------------------------------------------------------------------------------------------
int get_user_text (display_t* display,
                   char** string)
{
    assert (display);
    assert (string);

    display->cur_frame.amount_but = 0;

    char buffer[2 * MAX_USER_LEN_TEXT + 1] = "";
    int index = 0;
    int running = true;
    int cursor_view = false;
    SDL_Event event = {};
    Uint64 start_time = SDL_GetTicks ();

    SDL_StartTextInput ();

    while (running)
    {
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }

            if (event.type == SDL_TEXTINPUT)
            {
                // Что бы не было слишком больших строк
                if (index <= MAX_USER_LEN_TEXT)
                {
                    buffer[index++] = *(event.text.text);
                }
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (index != 0)
                    {
                        buffer[index--] = '\0';
                    }
                }

                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    buffer[index] = '\0';
                    running = 0;
                }
            }
        }

        if (running == false)
        {
            break;
        }

        if (SDL_GetTicks () - start_time >= TIME_DELAY_CURSOR)
        {
            cursor_view = !cursor_view;
            start_time = SDL_GetTicks ();
        }
                                    /* Для того, что бы курсор не вылезал за пределы строки */
        if (running == true && cursor_view == true && index < MAX_USER_LEN_TEXT)
        {
            buffer[index++] = '|';
            buffer[index] = '\0';
        }
        else
        {
            buffer[index++] = ' ';
            buffer[index] = '\0';
        }
        display->cur_frame.user_text = buffer;

        renew_display (display);
        display->cur_frame.user_text = NULL;
        index--;
    }

    printf ("ggg\n");
    SDL_StopTextInput ();

    return 0;
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция проверки нажатия кнопки
 * @param [in] button Указатель на структуру проверяемой кнопки
 * @param [in] event Текущий ивент
 * @return 1 (если была нажата), иначе 0
*/
// ----------------------------------------------------------------------------------------------------
int check_button_click (button_t* button,
                        SDL_Event* event)
{
    assert (button);
    assert (event);

    SDL_Rect but_place = PLACE_BUTTON_MASS[button->type];
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        int mouse_x = event->button.x;
        int mouse_y = event->button.y;

        if (mouse_x >= but_place.x &&
            mouse_x <= but_place.x + but_place.w &&
            mouse_y >= but_place.y &&
            mouse_y <= but_place.y + but_place.h)
        {
            return 1;
        }
    }
    return 0;
}
// ----------------------------------------------------------------------------------------------------




int main()
{
    display_t display = {};
    if (create_display(&display) != 0) {
        return 1;
    }

    int running = 1;
    EVENT current_event = START_PROGRAM;

    while (running)
    {
        current_event = get_event (current_event, &display);
        printf ("EVENT: %d\n", current_event);
        if (current_event == EXIT_PROGRAM)
        {
            running = 0;
        }
    }

    destroy_display (&display);

    return 0;
}

