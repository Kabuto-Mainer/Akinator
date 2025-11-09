#include <stdio.h>
#include <assert.h>
#include <ctype.h>
// #include <unistd.h>

#include "const.h"
#include "type.h"
#include "func.h"

// Start function for using tree functions
// --------------------------------------------------------------------------------------------------
int create_tree (tree_t* tree)
{
    tree->null = (node_t*) calloc (1, sizeof (node_t));
    if (tree->null == NULL)
    {
        EXIT_FUNC("NULL address", -1);
    }

                                // sizeof (char) in size_object
    tree->null->object = (obj_t) calloc (1, sizeof (START_OBJECT) / sizeof (START_OBJECT[0]));
    if (tree->null->object == BAD_OBJECT)
    {
        free (tree->null);
        EXIT_FUNC("NULL address", -1);
    }

    sprintf (tree->null->object, "%s", START_OBJECT);
    tree->null->hash = get_hash (tree->null->object);
    tree->null->father = NULL;
    tree->null->left = NULL;
    tree->null->right = NULL;
    tree->size++;

    tree->num_dump = find_number_dump ();

    return 0;
}
// --------------------------------------------------------------------------------------------------

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
    new_node->hash = get_hash (object);
    new_node->father = root;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}
// --------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------
node_t* get_next_node (node_t* node)
{
    assert (node);

    printf ("Is it %s? (Y/n)\n", node->object);

    int user_answer = get_user_bool ();
    if (user_answer == USER_YES)
    {
        return node->right;
    }

    else if (user_answer == USER_NO)
    {
        return node->left;
    }

    return NULL;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
node_t* find_node (node_t* node,
                   obj_t* object,
                   size_t* hash)
{
    assert (node);
    assert (object);
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
int get_user_bool ()
{
    while (1)
    {
        printf ("> ");
        char answer = getchar ();

        while (getchar() != '\n')
        {
            continue;
        }

        if (toupper(answer) == 'Y')
        {
            return USER_YES;
        }

        else if (toupper(answer) == 'N')
        {
            return USER_NO;
        }

        else
        {
            printf ("Please re-enter your answer (your answer mus be 'Y' or 'N'\n");
        }
    }
    return -1;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
obj_t get_user_object ()
{
    char buffer[200] = "";
    int amount_char = 0;

    printf ("> ");
    while (1)
    {
        if (scanf ("%[^\n]%n", buffer, &amount_char) != 1)
        {
            getchar ();
            printf ("\nPlease re-enter your answer (your answer should not contain any characters other than letters and numbers)\n");
            printf ("> ");
            continue;
        }

        //     printf ("STR: -%s-\n", buffer);
        // printf ("AMOUNT CHAR: %d\n", amount_char);
        getchar ();

        obj_t object = (obj_t) calloc (1, SIZE_OBJECT ((size_t) amount_char + 1));
        if (object == NULL)
        {
            EXIT_FUNC("NULL address", NULL);
        }
        sprintf (object, "%s", buffer);

        return object;
    }
    return BAD_OBJECT;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
int check_user_object (obj_t object)
{
    ASSERT_OBJECT(object);

    size_t size = get_size_object (object);
    size_t i = 0;

    for (i = 0; i < size; i++)
    {
        if ((toupper (object[i]) == 'N')
            && i + 1 < size)
        {
            if (toupper (object[i + 1]) != 'O')
            {
                continue;
            }

            if (i + 2 < size)
            {
                if (toupper (object[i + 2]) != ' ' && toupper (object[i + 2]) != 'T')
                {
                    continue;
                }

                if (i > 0)
                {
                    if (i + 3 < size)
                    {
                        if (toupper (object[i - 1]) == ' ' && (toupper (object[i + 2] == ' ' ||toupper (object[i + 3]) == ' ')))
                        {
                            break;
                        }
                    }

                    if (toupper (object[i - 1]) == ' ')
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
                    if (toupper (object[i - 1]) == ' ')
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

    printf ("Invalid string. Please do not use negation.\n");
    return -1;
}
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
EVENT get_event (EVENT prev)
{
    if (prev == NULL_EVENT)
    {
        printf ("Welcome to this program\n");
        printf ("Do you want to start?\n");

        if (get_user_bool == USER_YES)
        {
            return START_PROGRAM;
        }

        return EXIT_PROGRAM;
    }

    else if (prev == EXIT_PROGRAM)
    {
        printf ("Do you want to save new data base?\n");

                if (get_user_bool == USER_YES)
        {
            return SAVE_DATA;
        }

        return EXIT_PROGRAM;
    }

    else {
        printf ("What do you want to do?\n");
        printf (" - Guess an object (g)\n");
        printf (" - Compare the objects (c)\n");
        printf (" - Describe an object (d)\n");
        printf (" - Save data base (s)\n");
        printf (" - Exit program (e)\n");
    }
    char comand = '\0';

    while (1)
    {
        printf ("> ");
        comand = getchar();

        while (getchar() != '\n')
        {
            continue;
        }

        switch (toupper (comand))
        {
            case ('G')
            {
                return GUESS_OBJECT;
            }

            case ('C')
            {
                return COMPARE_OBJECTS;
            }

            case ('D')
            {
                return DESC_OBJECT;
            }

            case ('S')
            {
                return SAVE_DATA;
            }

            case ('E')
            {
                return EXIT_PROGRAM;
            }

            default
            {
                printf ("Please, re-enter you answer (you can use only 'g', 'c', 'd', 's', 'e')\n");
            }
        }
    }
    return EXIT_PROGRAM;
}
// --------------------------------------------------------------------------------------------------






// --------------------------------------------------------------------------------------------------
int delete_tree (tree_t* tree)
{
    assert (tree);

    delete_node (tree->null);

    return 0;
}
// --------------------------------------------------------------------------------------------------

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

    free (node->object);
    free (node);

    return 0;
}
// --------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------
int desc_object (tree_t* tree)
{
    assert (tree);

    printf ("Tell me the object and I will describe it.\n");
    obj_t user_object = get_user_object ();
    size_t hash = get_hash (user_object);

    node_t* user_node =



}
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
int guess_object (tree_t* tree)
{
    assert (tree);
    node_t* current_node = tree->null;

    printf ("Think of any character and I'll guess it.\n");
    while (1)
    {
        printf ("Is it %s? (Y/n)\n", current_node->object);
        int user_answer = get_user_bool ();

        if (current_node->left == NULL && current_node->right == NULL)
        {
            if (user_answer == USER_YES)
            {
                printf ("It couldn't be otherwise\n");
                return 1;
            }

            printf ("Are you sure?\n");
            user_answer = get_user_bool ();

            if (user_answer == USER_YES)
            {
                printf ("Then who did you guess?\n");
                obj_t user_object = get_user_object ();
                printf ("How is %s different from %s? %s is...\n",
                        current_node->object,
                        user_object,
                        user_object);

                obj_t category = {};
                while (1)
                {
                    category = get_user_object ();
                    if (check_user_object (category) == 0)
                    {
                        break;
                    }
                }
                create_node (current_node, LEFT_SIDE, current_node->object);
                create_node (current_node, RIGHT_SIDE, user_object);
                current_node->object = category;
                current_node->hash = get_hash (category);
                // printf ("1: %s\n2: %s\n3: %s\n",
                //         current_node->object, current_node->left->object, current_node->right->object);
                return 1;
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
    return -1;
}
// --------------------------------------------------------------------------------------------------
