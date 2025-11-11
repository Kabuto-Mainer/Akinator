#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
// #include <unistd.h>

#include "config.h"
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
    tree->null->rank = 0;
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
    new_node->rank = root->rank + 1;

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
                   obj_t object,
                   size_t hash)
{
    assert (node);
    ASSERT_OBJECT(object);

    if (node->hash == hash && strcmp (node->object, object) == 0)
    {
        return node;
    }

    if (node->left)
    {
        node_t* buffer = NULL;

        if ((buffer = find_node (node->left, object, hash)) != NULL)
        {
            return buffer;
        }
    }

    if (node->right)
    {
        node_t* buffer = NULL;

        if ((buffer = find_node (node->right, object, hash)) != NULL)
        {
            return buffer;
        }
    }

    return NULL;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
int save_node (node_t* node,
               FILE* file)
{
    assert (node);

    if (node->object == NULL)
    {
        fprintf (file, " nil ");
    }

    else
    {
        fprintf (file, "(\"%s\"", node->object);
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
                 "(nil)");
    }

    fprintf (file, ")");
    return 0;
}
// ----------------------------------------------------------------------------------------------------

// // ----------------------------------------------------------------------------------------------------
// int upload_node (node_t* root,
//                  FILE* file)
// {
//     assert (root);

//     char symbol = fgetchar (file);
//     if (symbol == '(')
//     {
//         char buffer[200] = "";
//         int amount_sym = 0;

//         if (fscanf (file, "%199[^(, )]%n",buffer, &amount_sym) != 1)
//         {
//             EXIT_FUNC("ERROR with reading\n", -1);
//         }

//         obj_t object = (obj_t) calloc (1, SIZE_OBJECT((size_t) amount_sym + 1));
//         if (object == BAD_OBJECT)
//         {
//             EXIT_FUNC("NULL calloc", -1);
//         }
//         sprintf (object, "%s", buffer);


//     }

//     else
//     {
//         printf ("ERROR in file\n");
//         return 1;
//     }
// }
// // ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
int get_user_bool ()
{
    while (1)
    {
        printf ("> ");
        int answer = getchar ();

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

// --------------------------------------------------------------------------------------------------
node_t* get_user_node (tree_t* tree)
{
    assert (tree);

    obj_t user_object = NULL;
    size_t hash = 0;
    node_t* user_node = NULL;

    while (1)
    {
        user_object = get_user_object ();
        hash = get_hash (user_object);

        user_node = find_node (tree->null, user_object, hash);
        free (user_object);

        if (user_node == NULL || (user_node->left != NULL && user_node->right != NULL))
        {
            if (user_node == NULL)
            {
                printf ("The object you entered was not found.\n");
            }

            else
            {
                printf ("The object you entered is category.\n");
            }

            printf ("Do you want to re-enter? (Y/n)\n");
            int user_answer = get_user_bool ();

            if (user_answer == USER_YES)
            {
                printf ("Your object:\n");
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
        printf ("Do you want to start? (Y\\n)\n");

        if (get_user_bool () == USER_YES)
        {
            return START_PROGRAM;
        }

        return EXIT_PROGRAM;
    }

    else if (prev == EXIT_PROGRAM)
    {
        printf ("Do you want to save new data base? (Y\\n)\n");

                if (get_user_bool () == USER_YES)
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
    int comand = '\0';

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

            case ('E'):
            {
                return EXIT_PROGRAM;
            }

            default:
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
int compare_objects (tree_t* tree)
{
    assert (tree);

    printf ("Tell me two objects and I will tell you difference between theirs.\n");
    printf ("Enter first object:\n");
    node_t* node_1 = get_user_node (tree);

    if (node_1 == NULL)
    {
        return 1;
    }
// Необходимо, что бы при первом NULL был return
    printf ("Enter second object:\n");
    node_t* node_2 = get_user_node (tree);

    if (node_2 == NULL)
    {
        return 1;
    }

    if (node_1 == node_2)
    {
        printf ("You enter just one object\n");
        return 0;
    }

    char left_side = 'l';
    char right_side = 'r';

    int size_1 = (int) node_1->rank;
    int size_2 = (int) node_2->rank;


    char* path_to_1 = (char*) calloc (size_t (size_1 + 1), sizeof (char));
    char* path_to_2 = (char*) calloc (size_t (size_2 + 1), sizeof (char));


    node_t* current_node = node_1->father;
    printf ("Node 1: %p\nNode 2: %p\n", node_1, node_2);
    for (int i = 0; i < size_1 - 1; i++)
    {
        if (current_node->father->right == current_node)
        {
            path_to_1[i] = right_side;
        }
// TODO Насколько тут важна надежность?
// TODO Надо ли нам проверять, не совпадают какие либо элементы в дереве
        else
        {
            path_to_1[i] = left_side;
        }
        current_node = current_node->father;
    }

    current_node = node_2->father;
    for (int i = 0; i < size_2 - 1; i++)
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
    }

    if (path_to_1[size_1] != path_to_2[size_2])
    {
        printf ("There are no common properties between the objects you entered\n");
    }

    else
    {
        printf ("What these objects have in common:\n");
        int index_1 = size_1;
        int index_2 = size_2;
        current_node = tree->null;
        while (path_to_1[index_1] != path_to_2[index_2])
        {
            printf (" - %s\n", current_node->object);

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

        printf ("The differences between these objects:\n");
        printf ("First is:\n");
        node_1 = current_node;
        while (index_1 != 0)
        {
            printf (" - %s\n", node_1->object);

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

        printf ("Second is:\n");
        node_2 = current_node;
        while (index_2 != 0)
        {
            printf (" - %s\n", node_2->object);

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
    }

    free (path_to_1);
    free (path_to_2);

    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
int desc_object (tree_t* tree)
{
    assert (tree);

    printf ("Tell me the object and I will describe it.\n");
    node_t* user_node = get_user_node (tree);

    if (user_node == NULL)
    {
        return 0;
    }

    node_t** path_to_object = (node_t**) calloc (size_t (user_node->rank + 1), sizeof (node_t*));

    node_t* current_node = user_node->father;
    for (int i = 0; i < (int) user_node->rank; i++)
    {
        path_to_object[i] = current_node;
        current_node = current_node->father;
    }

    printf ("%s is: \n", user_node->object);
    for (int i = (int) user_node->rank - 1; i >= 0; i--)
    {
        printf (" - %s\n", path_to_object[i]->object);
    }
    free (path_to_object);

    return 1;
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

            printf ("Are you sure? (Y\\n)\n");
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

// --------------------------------------------------------------------------------------------------
int save_data (tree_t* tree)
{
    assert (tree);

    printf ("Where do you want to save data base?\n");
    printf ("Do you want to save in standard directory (Y) or in own file (n)?\n");
    int user_answer = get_user_bool ();
    char address[200] = "";

    if (user_answer == USER_YES)
    {
        printf ("In what directory number do you want to save the file? (1-%d):\n", AMOUNT_DATA_DIR);

        int number_dir = 0;
        while (1)
        {
            printf ("> ");
            if (scanf ("%d", &number_dir) != 1)
            {
                printf ("Please, re-enter your answer.\n");
                continue;
            }

            if (number_dir < 1 || number_dir > AMOUNT_DATA_DIR)
            {
                printf ("Incorrect number directory. Please, re-enter it.\n");
                continue;
            }

            make_dir_address (ADDRESS_DATA_DIR, number_dir, "data.txt", address);
            break;
        }
    }

    else
    {
        printf ("Enter the file name (note that the file will be created in the current directory):\n");

        while (1)
        {
            int amount_sym = 0;
            char buffer[180] = "";

            printf ("> ");
            if (scanf ("%179s%n", buffer, &amount_sym) != 1)
            {
                printf ("Please, re-enter your address\n");
                continue;
            }

            for (int i = 0; i < amount_sym; i++)
            {
                if (address[i] == '/')
                {
                    printf ("Please, do not enter a directory. Just name file.\n");
                    continue;
                }
            }
            sprintf (address, "%s%s", ADDRESS_USER_DATA, buffer);
            break;
        }
    }

    FILE* file = fopen (address, "w");
    save_node (tree->null, file);
    fclose (file);

    printf ("File saved successfully\n");
    return 0;
}
// --------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------
// int import_data (tree_t* tree)
// {
//     assert (tree);

//     if (tree->null->left != NULL || tree->null->right != NULL)
//     {
//         printf ("Sorry, file upload is only possible if the tree is empty.\n");
//         return 0;
//     }

//     printf ("Where do you want to upload the file from? From a standard directory (Y) or from your own file (n) ?\n");
//     printf ("Do you want to save in standard directory (Y) or in own file (n) ?\n");
//     int user_answer = get_user_bool ();
//     char address[200] = "";

//     if (user_answer == USER_YES)
//     {
//         printf ("In what directory number do you want to upload the file? (1-%d):\n", AMOUNT_DATA_DIR);

//         int number_dir = 0;
//         while (1)
//         {
//             printf ("> ");
//             if (scanf ("%d", &number_dir) != 1)
//             {
//                 printf ("Please, re-enter your answer.\n");
//                 continue;
//             }

//             if (number_dir < 1 || number_dir > AMOUNT_DATA_DIR)
//             {
//                 printf ("Incorrect number directory. Please, re-enter it.\n");
//                 continue;
//             }

//             make_dir_address (ADDRESS_DATA_DIR, number_dir, "data.txt", address);
//             break;
//         }
//     }

//     else
//     {
//         printf ("Enter the file name (note that the file will be created in the current directory):\n");

//         while (1)
//         {
//             int amount_sym = 0;
//             char buffer[180] = "";

//             printf ("> ");
//             if (scanf ("%179s%n", buffer, &amount_sym) != 1)
//             {
//                 printf ("Please, re-enter your address\n");
//                 continue;
//             }

//             for (int i = 0; i < amount_sym; i++)
//             {
//                 if (address[i] == '/')
//                 {
//                     printf ("Please, do not enter a directory. Just name file.\n");
//                     continue;
//                 }
//             }
//             sprintf (address, "%s%s", ADDRESS_USER_DATA, buffer);
//             break;
//         }
//     }

//     FILE* file = fopen (address, "w");
//     save_node (tree->null, file);
//     fclose (file);

//     printf ("File saved successfully\n");
//     return 0;
// }
