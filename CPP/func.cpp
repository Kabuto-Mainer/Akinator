#include <stdio.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "../STACK/stack_define.h"
#include "../STACK/config.h"
#include "../STACK/stack.h"

#include "../H/display-config.h"
#include "../H/display.h"

#include "../H/config.h"
#include "../H/const.h"
#include "../H/type.h"
#include "../H/func.h"


// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция заполнения структуры дерева
 * @param [in] tree Адрес структуры дерева
*/
// --------------------------------------------------------------------------------------------------
int create_tree (tree_t* tree)
{
    assert (tree);

    tree->null = (node_t*) calloc (1, sizeof (node_t));
    if (tree->null == NULL)
    {
        EXIT_FUNC("NULL address", -1);
    }

    tree->null->object.name = strdup (START_OBJECT_NAME);
    tree->null->object.hash = get_hash (tree->null->object.name);
    tree->null->object.type_object = OWN_MEMORY;
    tree->null->father = NULL;
    tree->null->left = NULL;
    tree->null->right = NULL;
    tree->buffer = NULL;
    tree->size = 1;
    tree->num_dump = find_number_dump ();

    display_t* display = (display_t*) calloc (1, sizeof (display_t));
    if (display == NULL)
    {
        EXIT_FUNC("NULL calloc", -1);
    }

    create_display (display);
    tree->display = display;

    char address[200] = "";
    sprintf (address, "DUMPS/DUMP_%d/IMAGES", tree->num_dump);
    clean_dir (address);

    sprintf (address, "DUMPS/DUMP_%d/dump.html", tree->num_dump);
    clean_html (address);

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция обработки ивентов, полученных от пользователя
*/
// --------------------------------------------------------------------------------------------------
int akinator ()
{
    tree_t tree = {};
    create_tree (&tree);

    EVENT prev = NULL_EVENT;
    EVENT current = NULL_EVENT;
    while ((current = get_event (prev, tree.display)) != EXIT_PROGRAM)
    {
        switch ((int) current)
        {
            case GUESS_OBJECT:
            {
                guess_object (&tree);
                break;
            }

            case DESC_OBJECT:
            {
                desc_object (&tree);
                break;
            }

            case COMPARE_OBJECTS:
            {
                compare_objects (&tree);
                break;
            }

            case SAVE_DATA:
            {
                save_data (&tree);
                break;
            }

            case IMPORT_DATA:
            {
                import_data (&tree);
                break;
            }

            case START_PROGRAM:
            {
                break;
            }

            default:
            {
                EXIT_FUNC("Default case", -1);
            }
        }
        prev = current;
        dump_tree (&tree, "Dump after iteration");
    }

    PRINT_TEXT (tree.display, "Thank you for using the program");
    delete_tree (&tree);

    return 0;
}
// --------------------------------------------------------------------------------------------------



// -*************************************************************************************************



#ifndef STACK_FUNC
// ----------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция поиска объекта в базе данных
 * @param [in] node Указатель на узел
 * @param [in] object Объект, который и надо найти
 * @return node_t* (указатель на узел) - если узел найдет, иначе -> NULL
*/
// ----------------------------------------------------------------------------------------------------
node_t* find_node (node_t* node,
                   obj_t object)
{
    assert (node);
    ASSERT_OBJECT(object);

    int hash = object.hash;

    if (node->object.hash == hash && strcmp (node->object.name, object.name) == 0)
    {
        return node;
    }

    if (node->left)
    {
        node_t* buffer = NULL;

        if ((buffer = find_node (node->left, object)) != NULL)
        {
            return buffer;
        }
    }

    if (node->right)
    {
        node_t* buffer = NULL;

        if ((buffer = find_node (node->right, object)) != NULL)
        {
            return buffer;
        }
    }

    return NULL;
}
// ----------------------------------------------------------------------------------------------------
#endif // STACK_FUNC


#ifdef STACK_FUNC
// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция поиска объекта в базе данных с помощью цикла со стеком
 * @param [in] node Указатель на корень дерева
 * @param [in] object Объект, который и надо найти
 * @return node_t* (указатель на узел) - если узел найдет, иначе -> NULL
*/
// ----------------------------------------------------------------------------------------------------
node_t* find_node (node_t* node,
                   obj_t object)
{
    assert (node);
    ASSERT_OBJECT(object);

    stack_struct stack = {};
    stack_creator (&stack, 10, __FILE__, __LINE__, "Stack for find node");
    stack_push (&stack, node);

    int hash = object.hash;
    while (stack.size != 0)
    {
        node_t* current_node = NULL;
        stack_pop (&stack, &current_node);

        if (current_node->object.hash == hash)
        {
            stack_destruct (&stack);
            return current_node;
        }

        if (current_node->right != NULL)
        {
            stack_push (&stack, current_node->right);
        }

        if (current_node->left != NULL)
        {
            stack_push (&stack, current_node->left);
        }
    }
    stack_destruct (&stack);
    return NULL;
}
// ----------------------------------------------------------------------------------------------------
#endif // STACK_FUNC



// -*************************************************************************************************



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция получения узла по объекту, полученному от пользователя
 * @param [in] tree Указатель на структуру дерева
 * @return node_t* (если объект нашелся), иначе -> NULL
*/
// --------------------------------------------------------------------------------------------------
node_t* get_user_node (tree_t* tree)
{
    assert (tree);

    obj_t user_object = {};
    node_t* user_node = NULL;

    while (1)
    {
        user_object = get_user_object (tree->display);
        user_node = find_node (tree->null, user_object);
        free (user_object.name);

        if (user_node == NULL || (user_node->left != NULL && user_node->right != NULL))
        {
            if (user_node == NULL)
            {
                PRINT_TEXT (tree->display, "The object you entered was not found");
            }

            else
            {
                PRINT_TEXT (tree->display, "The personage you entered is category");
            }

            get_user_continue (tree->display);
            push_text (tree->display, "Do you want to re-enter?");
            int user_answer = get_user_bool (tree->display, "YES", "NO");

            if (user_answer == USER_YES)
            {
                push_text (tree->display, "Enter you personage:");
                continue;
            }

            else
            {
                return NULL;
            }
        }

        return user_node;
    }

    return NULL;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция записи изображения в текущий кадр
 * @param [in] display Указатель на структуру дисплея
 * @param [in] object Переданный объект
*/
// --------------------------------------------------------------------------------------------------
int push_image_object (display_t* display,
                       obj_t object)
{
    assert (display);
    ASSERT_OBJECT (object);

    if (object.image == NULL)
    {
        return 0;
    }

    display->cur_frame.obj_img = object.image;
    return 0;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция удаления текстуры объекта из окна
 * @param [in] display Указатель на структуру дисплея
*/
// --------------------------------------------------------------------------------------------------
int pop_image_object (display_t* display)
{
    assert (display);

    display->cur_frame.obj_img = NULL;
    return 0;
}
// --------------------------------------------------------------------------------------------------



// -*************************************************************************************************



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция удаления всего дерева
 * @param [in] tree Указатель на структуру дерева
*/
// --------------------------------------------------------------------------------------------------
int delete_tree (tree_t* tree)
{
    assert (tree);

    delete_node (tree->null);

    if (tree->buffer != NULL)
    {
        free (tree->buffer);
    }

    destroy_display (tree->display);
    free (tree->display);
    return 0;
}
// --------------------------------------------------------------------------------------------------

#ifndef STACK_FUNC
// --------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция удаления элемента
 * @param [in] node Указатель на текущий удаляемый узел
 * @return 0
*/
// --------------------------------------------------------------------------------------------------
int delete_node (node_t* node)
{
    assert (node);

    if (node->left)
    {
        delete_node (node->left);
    }

    if (node->right)
    {
        delete_node (node->right);
    }

    if ((node->object.type_object & OWN_MEMORY) == OWN_MEMORY)
    {
        free (node->object.name);
    }
    free (node);

    return 0;
}
// --------------------------------------------------------------------------------------------------
#endif // STACK_FUNC

#ifdef STACK_FUNC
// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция удаления элемента с помощью цикла и стека
 * @param [in] node Указатель на корень дерева
 * @return 0
*/
// --------------------------------------------------------------------------------------------------
int delete_node (node_t* node)
{
    assert (node);

    stack_struct stack = {};
    stack_creator (&stack, 10, __FILE__, __LINE__, "Stack for delete tree");
    stack_push (&stack, node);

    while (stack.size != 0)
    {
        node_t* current_node = NULL;
        stack_pop (&stack, &current_node);

        if (current_node->left != NULL)
        {
            stack_push (&stack, current_node->left);
        }

        if (current_node->right != NULL)
        {
            stack_push (&stack, current_node->right);
        }

        if ((current_node->object.type_object & OWN_MEMORY) == OWN_MEMORY)
        {
            free (current_node->object.name);

            if (current_node->object.image != NULL)
            {
                free (current_node->object.image);
            }

            if (current_node->object.audio != NULL)
            {
                free (current_node->object.audio);
            }
        }
        free (current_node);
    }

    stack_destruct (&stack);
    return 0;
}
// --------------------------------------------------------------------------------------------------
#endif // STACK_FUNC



// -*************************************************************************************************



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция описания объекта
 * @param [in] tree Указатель на структуру дерева
*/
// --------------------------------------------------------------------------------------------------
int desc_object (tree_t* tree)
{
    assert (tree);

    push_text (tree->display, "Tell me the personage and I will describe it");
    node_t* user_node = get_user_node (tree);

    if (user_node == NULL)
    {
        return 0;
    }

    stack_struct stack = {};
    stack_creator (&stack, 10, __FILE__, __LINE__, "stack for find");
    stack_push (&stack, NULL);

    node_t* current_node = user_node->father;

    int len = 0;
    while (current_node != NULL)
    {
        stack_push (&stack, current_node);
        current_node = current_node->father;
        len++;
    }

    push_text (tree->display, "%s: ", user_node->object.name);

    node_t* prev = NULL;
    stack_pop (&stack, &prev);
    for (int i = 0; i < len; i++)
    {
        stack_pop (&stack, &current_node);

        if (current_node == prev->left)
        {
            add_text (tree->display, " - isn't %s,", prev->object.name);
        }

        else
        {
            add_text (tree->display, " - is %s,", prev->object.name);
        }
        prev = current_node;
    }
    stack_destruct (&stack);

    renew_display (tree->display);
    get_user_continue (tree->display);

    return 1;
}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция сравнения объектов, реализованная на и цикле со стеком, и на обычном динамическом массиве
 * @param [in] tree Указатель на структуру дерева
 * @note Тип обработки данных настраивается через STACK_FUNC (если объявлен, то используется стек, иначе -> динамический массив
*/
// --------------------------------------------------------------------------------------------------
int compare_objects (tree_t* tree)
{
    assert (tree);

    if (tree->size < 3)
    {
        PRINT_TEXT (tree->display, "Not enough personage.");
        return 1;
    }
    PRINT_TEXT (tree->display, "Tell me two personages and I will\n tell you difference between theirs.");
    get_user_continue (tree->display);
    push_text (tree->display, "Enter first personage:");
    node_t* node_1 = get_user_node (tree);

    if (node_1 == NULL)
    {
        return 1;
    }
// Необходимо, что бы при первом NULL был return
    push_text (tree->display, "Enter second personage:");
    node_t* node_2 = get_user_node (tree);

    if (node_2 == NULL)
    {
        return 1;
    }

    if (node_1 == node_2)
    {
        PRINT_TEXT (tree->display, "You enter just one personage");
        get_user_continue (tree->display);
        return 0;
    }

#ifdef STACK_FUNC
    stack_struct stack_node_1 = {};
    stack_creator (&stack_node_1, 10, __FILE__, __LINE__, "Stack of node 1");
    stack_push (&stack_node_1, node_1);

    stack_struct stack_node_2 = {};
    stack_creator (&stack_node_2, 10, __FILE__, __LINE__, "Stack of node 2");
    stack_push (&stack_node_2, node_2);

    node_t* buffer_node_1 = node_1->father;
    node_t* buffer_node_2 = node_2->father;

    int len_path_1 = 0;
    int len_path_2 = 0;

    while (buffer_node_1->father != NULL) // Можно через for, но как то уже все такие функции делал через while
    {
        stack_push (&stack_node_1, buffer_node_1);
        buffer_node_1 = buffer_node_1->father;
        len_path_1++;
    }

    while (buffer_node_2->father != NULL)
    {
        stack_push (&stack_node_2, buffer_node_2);
        buffer_node_2 = buffer_node_2->father;
        len_path_2++;
    }

    int amount_common = 0;
    PRINT_TEXT (tree->display, "What these personage have in common:");
    get_user_continue (tree->display);

    node_t* next_1 = NULL;
    node_t* next_2 = NULL;
    stack_pop (&stack_node_1, &next_1);
    stack_pop (&stack_node_2, &next_2);
    push_text (tree->display, "They: ");

    while (next_1 == next_2)
    {

        if (buffer_node_1->left == next_2)
        {
            add_text (tree->display, " isn't %s, ", buffer_node_1->object.name);
        }


        else
        {
            add_text (tree->display, " is %s, ", buffer_node_1->object.name);
        }

        buffer_node_1 = next_1;
        buffer_node_2 = next_2;
        stack_pop (&stack_node_1, &next_1);
        stack_pop (&stack_node_2, &next_2);
        amount_common++;
    }

    stack_push (&stack_node_1, next_1);
    stack_push (&stack_node_2, next_2);

    get_user_continue (tree->display);
    PRINT_TEXT (tree->display, "The differences between these personage:");
    get_user_continue (tree->display);
    push_text (tree->display, "First:");

    int buffer_len = amount_common;
    while (buffer_len <= len_path_1)
    {
        if (buffer_node_1->left != next_1)
        {
            add_text (tree->display, " - isn't %s,", buffer_node_1->object.name);
        }

        else
        {
            add_text (tree->display, " - is %s,", buffer_node_1->object.name);
        }
        buffer_node_1 = next_1;
        stack_pop (&stack_node_1, &next_1);
        buffer_len++;
    }
    renew_display (tree->display);
    get_user_continue (tree->display);

    push_text (tree->display, "Second is:");

    buffer_len = amount_common;
    while (buffer_len <= len_path_2)
    {
        if (buffer_node_2->left != next_2)
        {
            add_text (tree->display, " - isn't %s,", buffer_node_2->object.name);
        }

        else
        {
            add_text (tree->display, " - is %s,", buffer_node_2->object.name);
        }
        buffer_node_2 = next_2;
        stack_pop (&stack_node_2, &next_2);
        buffer_len++;
    }
    get_user_continue (tree->display);

    stack_destruct (&stack_node_1);
    stack_destruct (&stack_node_2);
#endif // STACK_FUNC

#ifndef STACK_FUNC
    char left_side = 'l';
    char right_side = 'r';

    int size = (int) tree->size;
    int size_1 = 0;
    int size_2 = 0;

    char* path_to_1 = (char*) calloc (size_t (size), sizeof (char));
    char* path_to_2 = (char*) calloc (size_t (size), sizeof (char));

    if (path_to_1 == NULL || path_to_2 == NULL)
    {
        EXIT_FUNC ("NULL calloc", -1);
    }

    node_t* current_node = node_1->father;
    // printf ("Node 1: %p\nNode 2: %p\n", node_1, node_2);
    for (int i = 0; i < size - 1; i++)
    {
// TODO Насколько тут важна надежность?
// TODO Надо ли нам проверять, не совпадают какие либо элементы в дереве

        if (current_node->father->right == current_node)
        {
            path_to_1[i] = right_side;
        }
        else
        {
            path_to_1[i] = left_side;
        }

        current_node = current_node->father;
        size_1++;

        if (current_node->father == NULL)
        {
            break;
        }
    }

    current_node = node_2->father;
    for (int i = 0; i < size - 1; i++)
    {
        if (current_node->father->right == current_node)
        {
            path_to_2[i] = right_side;
        }

        else
        {
            path_to_2[i] = left_side;
        }

        current_node = current_node->father;
        size_2++;

        if (current_node->father == NULL)
        {
            break;
        }
    }

    my_print ("What these objects have in common:\n");
    int index_1 = size_1;
    int index_2 = size_2;
    current_node = tree->null;
    while (path_to_1[index_1] != path_to_2[index_2])
    {
        if (current_node == current_node->left->father)
        {
            my_print (" - isn't %s\n", current_node->object.name);
        }

        else
        {
            my_print (" - is %s\n", current_node->object.name);
        }

        if (path_to_1[index_1] == left_side)
        {
            current_node = current_node->left;
        }

        else
        {
            current_node = current_node->right;
        }

        index_1--;
        index_2--;
    }

    my_print ("The differences between these objects:\n");
    my_print ("First is:\n");
    node_1 = current_node;
    while (index_1 != 0)
    {
        my_print (" - %s\n", node_1->object.name);

        if (path_to_1[index_1] == left_side)
        {
            node_1 = node_1->left;
        }

        else
        {
            node_1 = node_1->right;
        }

        index_1--;
    }

    my_print ("Second is:\n");
    node_2 = current_node;
    while (index_2 != 0)
    {
        my_print (" - %s\n", node_2->object.name);

        if (path_to_2[index_2] == left_side)
        {
            node_2 = node_2->left;
        }

        else
        {
            node_2 = node_2->right;
        }

        index_1--;
    }

    free (path_to_1);
    free (path_to_2);
#endif // STACK_FUNC

    return 0;

}
// --------------------------------------------------------------------------------------------------



// -*************************************************************************************************



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция отгадывания пользовательского объекта и, при необходимости, добавления его в базу данных
 * @param [in] tree Указатель на структуру дерева
*/
// --------------------------------------------------------------------------------------------------
int guess_object (tree_t* tree)
{
    assert (tree);
    node_t* current_node = tree->null;

    PRINT_TEXT (tree->display, "Think of any character and I'll guess it");
    get_user_continue (tree->display);

    while (true)
    {
        push_text (tree->display, "Is it %s?", current_node->object.name);
        push_image_object (tree->display, current_node->object);
        play_object_audio (tree->display, current_node->object);

        int user_answer = get_user_bool (tree->display, "YES", "NO");

        if (current_node->left == NULL && current_node->right == NULL)
        {
            if (user_answer == USER_YES)
            {
                PRINT_TEXT (tree->display, "It couldn't be otherwise");
                pop_image_object (tree->display);
                return 0;
            }

            pop_image_object (tree->display);
            push_text (tree->display, "Are you sure?");
            user_answer = get_user_bool (tree->display, "YES", "NO");

            if (user_answer == USER_YES)
            {
                push_text (tree->display, "Then who did you guess?");
                obj_t user_object = get_user_object (tree->display);
                get_image_audio (tree->display, &user_object);

                push_text (tree->display, "How is %s different from %s?\n %s is...",
                                           current_node->object.name,
                                           user_object.name,
                                           user_object.name);

                obj_t category = {};
                while (1)
                {
                    category = get_user_object (tree->display);
                    if (check_user_object (category) == 0)
                    {
                        break;
                    }

                    PRINT_TEXT (tree->display, "Invalid string. Please do not use negation.");
                }

                get_image_audio (tree->display, &category);

                PRINT_TEXT (tree->display, "I took note of this.");
                create_node (current_node, LEFT_SIDE, current_node->object);
                create_node (current_node, RIGHT_SIDE, user_object);
                current_node->object = category;
                // printf ("1: %s\n2: %s\n3: %s\n",
                //         current_node->object, current_node->left->object, current_node->right->object);
                return 0;
            }

            continue;
        }

        if (user_answer == USER_YES)
        {
            current_node = current_node->right;
        }

        else
        {
            current_node = current_node->left;
        }
    }
    return 1;
}
// --------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция получения пользовательского объекта и сохранение его в динамическую память
 * @return obj_t - пользовательский объект
 * @note Сохраняет в динамическую память -> необходимо с дальнейшем будет самому освободить
*/
// ----------------------------------------------------------------------------------------------------
obj_t get_user_object (display_t* display)
{
    char* name = NULL;
    obj_t object = {};

    get_user_text (display, &name);

    object.name = name;
    object.hash = get_hash (object.name);
    object.type_object = OWN_MEMORY;

    return object;
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция проверки корректности пользовательской строки на предмет наличия отрицания
 * @param [in] object Пользовательский объект, направленный на проверку
 * @return 0 - если корректный ввод, иначе -1
*/
// ----------------------------------------------------------------------------------------------------
int check_user_object (obj_t object)
{
// TODO: переделать алгос
    ASSERT_OBJECT(object);

    size_t size = get_len_name (object);
    size_t i = 0;

    char* name = object.name;
    for (i = 0; i < size; i++)
    {
        if ((toupper (name[i]) == 'N')
            && i + 1 < size)
        {
            if (toupper (name[i + 1]) != 'O')
            {
                continue;
            }

            if (i + 2 < size)
            {
                if (toupper (name[i + 2]) != ' ' && toupper (name[i + 2]) != 'T')
                {
                    continue;
                }

                if (i > 0)
                {
                    if (i + 3 < size)
                    {
                        if (toupper (name[i - 1]) == ' ' && (toupper (name[i + 2] == ' ' || toupper (name[i + 3]) == ' ')))
                        {
                            break;
                        }
                    }

                    if (toupper (name[i - 1]) == ' ')
                    {
                        break;
                    }

                    continue;
                }

                else
                {
                    break;
                }
            }

            else
            {
                if (i > 0)
                {
                    if (toupper (name[i - 1]) == ' ')
                    {
                        break;
                    }

                    continue;
                }

                else
                {
                    break;
                }
            }
        }
    }

    if (i == size)
    {
        return 0;
    }

    return -1;
}
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Создание узла и добавление связей
 *
 * @param [in] root Родительский корень
 * @param [in] side Сторона, куда нужно добавлять новый элемент
 * @param [in] object Добавляемый объект
 * @return node_t* Указатель на новый узел
 */
// --------------------------------------------------------------------------------------------------
node_t* create_node (node_t* root,
                     SIDES side,
                     obj_t object)
{
    assert (root);
    ASSERT_OBJECT(object);

    node_t* new_node = (node_t*) calloc (1, sizeof (node_t));
    if (new_node == NULL)
    {
        EXIT_FUNC("NULL address", NULL);
    }

    if (side == LEFT_SIDE)
    {
        root->left = new_node;
    }

    else
    {
        root->right = new_node;
    }

    new_node->object = object;
    new_node->father = root;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания адреса аудио файла для его расположения в стандартной директории
 * @param [in] buffer Указатель на адрес
 * @param [in] name_obj Имя объекта -> Имя файла
*/
// --------------------------------------------------------------------------------------------------
int create_adr_audio (char* buffer,
                      const char* name_obj)
{
    assert (buffer);
    assert (name_obj);

    int index = 0;
    sprintf (buffer, "%s", OBJECT_AUDIO);
    index += (int) strlen (OBJECT_AUDIO);

    while (*name_obj != '\0')
    {
        if (*name_obj == ' ')
        {
            buffer[index++] = '_';
        }

        else
        {
            buffer[index++] = *name_obj;
        }

        name_obj++;
    }
    strcat (buffer + index, ".wav\0");

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция проигрывания аудио-дорожки объекта
 * @param [in] display Указатель на структуру дисплея
 * @param [in] object Полученный объект
*/
// --------------------------------------------------------------------------------------------------
int play_object_audio (display_t* display,
                       obj_t object)
{
    assert (display);
    ASSERT_OBJECT(object);

    if (object.audio != NULL)
    {
        play_audio (display->audio_data.play, object.audio);
    }

    return 0;
}
// --------------------------------------------------------------------------------------------------



// -*************************************************************************************************



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция сохранения базы данных либо в стандартную директорию, либо в пользовательский файл
 * @param [in] tree Указатель на структуру дерева
 * @return 0
*/
// --------------------------------------------------------------------------------------------------
int save_data (tree_t* tree)
{
    assert (tree);

    push_text (tree->display, "Where do you want to save data base?");
    int user_answer = get_user_bool (tree->display, "STANDARD DIR", "OWN FILE");
    char address[200] = "";

    if (user_answer == USER_YES)
    {
        push_text (tree->display,
                  "In what directory number\n do you want to save the file? (1-%d):",
                  AMOUNT_DATA_DIR);

        int number_dir = 0;
        while (1)
        {
            char* adr = NULL;
            get_user_text (tree->display, &adr);

            if (sscanf (adr, "%d", &number_dir) != 1)
            {
                free (adr);
                PRINT_TEXT (tree->display, "Please, re-enter your answer.");
                continue;
            }
            free (adr);

            if (number_dir < 1 || number_dir > AMOUNT_DATA_DIR)
            {
                PRINT_TEXT (tree->display, "Incorrect number directory.\n Please, re-enter it.");
                continue;
            }

            make_dir_address (ADDRESS_DATA_DIR, number_dir, "data.txt", address);
            break;
        }
    }

    else
    {
        push_text (tree->display, "Enter file name");

        while (1)
        {
            int amount_sym = 0;
            char* adr = NULL;

            get_user_text (tree->display, &adr);

            if (sscanf (adr, "%s%n", address, &amount_sym) != 1)
            {
                free (adr);
                push_text (tree->display, "Do you want to use own file?");

                if (get_user_bool (tree->display, "YES", "NO") != USER_YES)
                {
                    return 0;
                }
                push_text (tree->display , "Please, re-enter your address\n");
                continue;
            }
            free (adr);

            for (int i = 0; i < amount_sym; i++)
            {
                if (address[i] == '/')
                {
                    push_text (tree->display, "Please, do not enter a directory. Just name file.\n");
                    continue;
                }
            }
            // sprintf (address, "%s%s", ADDRESS_USER_DATA, buffer);
            address[amount_sym + 1] = '\0';
            break;
        }
    }

    FILE* file = fopen (address, "w");
    if (file == NULL)
    {
        EXIT_FUNC("NULL file", 1);
    }

    save_node (tree->null, file);
    fclose (file);

    PRINT_TEXT (tree->display, "File saved successfully\n");
    return 0;
}
// --------------------------------------------------------------------------------------------------
// todo: check file
// ----------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция, сохраняющая узел в файл
 * @param [in] node Узел, который будет сохраняться
 * @param [in] file Указатель на файл, куда будет записываться база данных
 * @return 0
*/
// ----------------------------------------------------------------------------------------------------
int save_node (node_t* node,
               FILE* file)
{
    assert (node);

    if (node->object.name == NULL)
    {
        fprintf (file, " nil ");
        return 0;
    }

    else
    {
        fprintf (file, "(\"%s\" ", node->object.name);

        if (node->object.image != NULL)
        {
            fprintf (file, "[\"%s\", ", node->object.image);
        }

        if (node->object.audio != NULL)
        {
            fprintf (file, "[\"%s\", ", node->object.audio);
        }
        fprintf (file, "] ");
    }


    if (node->left)
    {
        save_node (node->left, file);
    }

    else
    {
        fprintf (file,
                 " nil ");
    }

    if (node->right)
    {
        save_node (node->right, file);
    }

    else
    {
        fprintf (file,
                 " nil ");
    }

    fprintf (file, ")");
    return 0;
}
// ----------------------------------------------------------------------------------------------------



// -*************************************************************************************************



// --------------------------------------------------------------------------------------------------
/**
 * @brief Функция, загружающая базу данных из выбранной пользователем директории, либо из введенного файла
 * @param [in] tree Дерево, в которое загружается база данных, предварительно освобождая его
 * @return 1
 */
// --------------------------------------------------------------------------------------------------
int import_data (tree_t* tree)
{
    assert (tree);

    if (tree->null->left != NULL || tree->null->right != NULL)
    {
        PRINT_TEXT (tree->display, "Sorry, file upload is only possible\n if the tree is empty.");
        return 0;
    }

    push_text (tree->display, "Where do you want to upload the file from?");
    int user_answer = get_user_bool (tree->display, "STANDARD DIR", "OWN FILE");
    char address[200] = "";

    if (user_answer == USER_YES)
    {
        push_text (tree->display, "In what directory number \ndo you want to upload the file? (1-%d):", AMOUNT_DATA_DIR);

        int number_dir = 0;
        while (1)
        {
            char* adr = NULL;
            get_user_text (tree->display, &adr);

            if (sscanf (adr, "%d", &number_dir) != 1)
            {
                push_text (tree->display, "Please, re-enter your answer.");
                free (adr);
                continue;
            }
            free (adr);

            if (number_dir < 1 || number_dir > AMOUNT_DATA_DIR)
            {
                push_text (tree->display, "Incorrect number directory.\nDo you what to continue");
                if (get_user_bool (tree->display, "YES", "NO") == USER_NO)
                {
                    return 0;
                }

                continue;
            }

            make_dir_address (ADDRESS_DATA_DIR, number_dir, "data.txt", address);
            break;
        }
    }

    else
    {
        push_text (tree->display, "Enter file name");

        while (1)
        {
            int amount_sym = 0;
            char buffer[100] = "";
            char* adr = NULL;

            get_user_text (tree->display, &adr);

            if (sscanf (adr, "%s%n", buffer, &amount_sym) != 1)
            {
                free (adr);

                push_text (tree->display, "Incorrect input\nDo you want to continue?");
                if (get_user_bool (tree->display, "YES", "NO") == USER_NO)
                {
                    return 0;
                }
                push_text (tree->display, "Please, re-enter your address\n");
                continue;
            }
            free (adr);


            for (int i = 0; i < amount_sym; i++)
            {
                if (buffer[i] == '/')
                {
                    push_text (tree->display, "Please, do not enter a directory. Only name file.");
                    continue;
                }
            }

            // Проверка, существует ли файл
            if (access (buffer, F_OK) != 0)
            {
                push_text (tree->display, "File does not exist.");
                continue;
            }

            sprintf (address, "%s%s", ADDRESS_USER_DATA, buffer);
            break;
        }
    }


    FILE* file = fopen (address, "r");
    if (file == NULL)
    {
        EXIT_FUNC ("NULL file", -1);
    }

    size_t size = file_size_check (address);
    char* buffer = (char*) calloc (size + 1, sizeof (char));
    if (buffer == NULL)
    {
        EXIT_FUNC("NULL calloc", -1);
    }

    size_t amount_char = fread (buffer, size, sizeof (char), file);
    (void) amount_char;
    buffer[size] = '\0';

    clean_html (ADDRESS_DUMP_DEBUG "dump.html");
    clean_dir (ADDRESS_DUMP_DEBUG "IMAGES");

    delete_node (tree->null);
    tree->buffer = buffer;

    int size_tree = 0;
    tree->null = upload_node (NULL, &(buffer), &size_tree);
    tree->size = (size_t) size_tree;
    fclose (file);

    PRINT_TEXT (tree->display, "File import successfully");
    return 0;
}
// --------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция создания узла по буферу
 * @param [in] root Родительский корень
 * @param [in] cur_char Указатель на адрес буфера
 * @param [in] size_tree Указатель на размер дерева
 * @return node_t* Указатель на новый узел
 */
// ----------------------------------------------------------------------------------------------------
node_t* upload_node (node_t* root,
                     char** cur_char,
                     int* size_tree)
{
    // assert (root);
    assert (cur_char);
    assert (*cur_char);

    // debug_html (root, "Dump before skip void", *cur_char);
    skip_void (cur_char);
    if (**cur_char == '(')
    {
        node_t* new_node = (node_t*) calloc (1, sizeof (node_t));
        if (new_node == NULL)
        {
            EXIT_FUNC("NULL calloc", NULL);
        }
        (*size_tree)++;

        (*cur_char) += 2; // Skip - (" -
        new_node->object.name = *cur_char;


        skip_after_symbol (cur_char, '"');
        debug_html (root, "Dump after skip_after_symbol", *cur_char);
        skip_void (cur_char);

        new_node->object.hash = get_hash (new_node->object.name);
        new_node->object.type_object = NO_OWN_MEMORY;

    // Есть ли доп. файлы для объекта
        if (**cur_char == '[')
        {
            // printf ("STR: %s\n", *cur_char);
            (*cur_char) += 2; // Skip -["-
            new_node->object.image = *cur_char;

            debug_html (root, "Dump before skip_after_symbol (with add objects)", *cur_char);

            skip_after_symbol (cur_char, '"');
            // (*cur_char)++; // Skip -"-
            skip_void (cur_char);

            debug_html (root, "Dump before skip_after_symbol (with add objects)", *cur_char);
            new_node->object.type_object |= IS_IMAGE;

            if ((**cur_char) != ']')
            {
                (*cur_char)++;
                new_node->object.audio = *cur_char;
                skip_after_symbol (cur_char, '"');
                (*cur_char) += 2; // Skip -"-
                skip_void (cur_char);

                new_node->object.type_object |= IS_AUDIO;
            }
            (*cur_char)++; // Skip -]-
        }

        else
        {
            new_node->object.audio = NULL;
            new_node->object.image = NULL;
        }

        skip_void (cur_char);
        new_node->left = upload_node (new_node, cur_char, size_tree);
        new_node->right = upload_node (new_node, cur_char, size_tree);

        new_node->father = root;

        (*cur_char)++; // Skip -)-
        return new_node;
    }

    else
    {
        debug_html (root, "Dump before check nil", *cur_char);

        int int_nil_enter = 0;
        int int_nil_const = 0;

        memcpy (&int_nil_enter, *cur_char, sizeof (int));
        memcpy (&int_nil_const, "nil ", sizeof (int));

        if (int_nil_const == int_nil_enter)
        {
            *cur_char += 3; // Skip -nil -
            skip_void (cur_char);
            debug_html (root, "Dump after check nil", *cur_char);
            return NULL;
        }

        printf ("ERROR in data: incorrect syntax -%s-\n", *cur_char);
        return NULL;
    }
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция скипа cur_char до символа, замена его на '\0' и перевод cur_char на следующий
 * @param [in] cur_pose Указатель на адрес хранения текущего символа
 * @param [in] symbol Символ, который будет заменяться на '\0'
*/
// ----------------------------------------------------------------------------------------------------
int skip_after_symbol (char** cur_pose,
                       int symbol)
{
    assert (cur_pose);
    assert (*cur_pose);

    *cur_pose = strchr (*cur_pose, symbol);
    // printf ("STR: %s\n", *cur_pose);
    **cur_pose = '\0';
    (*cur_pose)++;

    skip_void (cur_pose);

    return 0;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
/**
 * @brief Функция скипа всего ненужного синтаксиса, указанного в SYNTAX_VOID
 * @param [in] Указатель на адрес хранения текущего символа
*/
// ----------------------------------------------------------------------------------------------------
int skip_void (char** cur_pose)
{
    assert (cur_pose);
    assert (*cur_pose);

    while (1)
    {
        int flag = 0;
        for (size_t i = 0; i < sizeof (SYNTAX_VOID) * sizeof (SYNTAX_VOID[0]); i++)
        {
            if (**cur_pose == SYNTAX_VOID[i])
            {
                (*cur_pose)++;
                break;
            }
        }

        if (flag == 0)
        {
            return 0;
        }
    }
    return -1;
}
// ----------------------------------------------------------------------------------------------------



// -*************************************************************************************************








