#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "type.h"
#include "func.h"

static int AMOUNT_IMAGES = 0;


// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция дампа всего дерева
 * @param [in] tree Указатель на структуру всего дерева
 * @param [in] reason Причина вызова дампа
 * @return 0
*/
// -------------------------------------------------------------------------------------------------------
int dump_tree (tree_t* tree,
               const char* reason)
{
    assert (tree);

    create_graph (tree);

    char address_dot[200] = {};
    make_dir_address (ADDRESS_DUMP_DIR, tree->num_dump, "graph.dot", address_dot);

    char address_img[200] = {};
    make_dir_address (ADDRESS_DUMP_DIR, tree->num_dump, "IMAGES/img_", address_img);

    char comand[200] = {};
    sprintf (comand,
             "dot %s -T png -o %s%d.png",
             address_dot,
             address_img,
             AMOUNT_IMAGES);

    int trash = system (comand);
    (void) trash;

    // sprintf (address,
    //          "%s_%d/dump.html",
    //          ADDRESS_DUMP_DIR,
    //          tree->create.number_dir);

    // create_html (tree, address, reason);
    (void) reason;
    AMOUNT_IMAGES++;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания графа по дереву
 * @param [in] tree Указатель на причину дампа
 * @return 0 (если не было ошибок), иначе -> -1
*/
// -------------------------------------------------------------------------------------------------------
int create_graph (tree_t* tree)
{
    assert (tree);

    char address[200] = "";
    make_dir_address (ADDRESS_DUMP_DIR, tree->num_dump, "graph.dot", address);

    FILE* file = fopen (address, "w");
    if (file == NULL)
    {
        EXIT_FUNC("NULL file", -1);
    }

    fprintf (file,
             "digraph {\n"
             "  rankdir=UD;\n"
             "  bgcolor=\"#1e1e1e\"\n"
             "  splines=ortho;\n"
             "  nodesep=0.4;\n"
             "  ranksep=0.6;\n"
             "  node [shape=plaintext, style=filled, fontname=\"Helvetica\"];\n"
             "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    create_block (tree->null, file);
    create_line (tree->null, file);

    fprintf (file, "\n}\n");

    fclose (file);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания графа для debug
 * @param [in] root Узел, с которого строится граф
 * @return 0 (если не было ошибок), иначе -> -1
 * @note Граф сохраняется в свою директорию ADDRESS_DUMP_DEBUG
*/
// -------------------------------------------------------------------------------------------------------
int debug_graph (node_t* root)
{
    assert (root);

    FILE* file = fopen (ADDRESS_DUMP_DEBUG "graph.dot", "w");
    if (file == NULL)
    {
        EXIT_FUNC("NULL file", -1);
    }

    fprintf (file,
             "digraph {\n"
             "  rankdir=UD;\n"
             "  bgcolor=\"#1e1e1e\"\n"
             "  splines=ortho;\n"
             "  nodesep=0.4;\n"
             "  ranksep=0.6;\n"
             "  node [shape=plaintext, style=filled, fontname=\"Helvetica\"];\n"
             "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    create_block (root, file);
    create_line (root, file);

    fprintf (file, "\n}\n");

    fclose (file);
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция создания блоков графа
 * @param [in] node Указатель на текущий узел
 * @param [in] file Указатель на dot файл
 * @return 0
*/
// -------------------------------------------------------------------------------------------------------
int create_block (node_t* node,
                  FILE* file)
{
    assert (node);

    // printf ("NAME: %s\n", node->object.name);
    char color_object[20] = "";
    sprintf (color_object, "#ed25fff0");

    char left_line[20] = "NULL";
    char right_line[20] = "NULL";

    if (node->left)
    {
        sprintf (left_line, "%p", node->left);
        sprintf (color_object, "#f46b8bff");
        create_block (node->left, file);
    }

    if (node->right)
    {
        sprintf (right_line, "%p", node->right);
        sprintf (color_object, "#f46b8bff");
        create_block (node->right, file);
    }


    fprintf (file,
             "block_%p [label=<\n<TABLE CELLSPACING=\"0\" CELLPADDING=\"4\">\n"
             "<TR><TD PORT=\"root\" BGCOLOR=\"#0308f9ff\" COLSPAN=\"2\"><B>%p</B></TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\" COLSPAN=\"2\">%s</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#b209ccff\" COLSPAN=\"2\">ROOT=%p</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#ff7301ff\">NO</TD><TD BGCOLOR=\"#08ff3aff\">YES</TD></TR>\n"

             "<TR>\n<TD PORT=\"left\" BGCOLOR=\"#ff7301ff\">%s</TD>\n"
             "<TD PORT=\"right\" BGCOLOR=\"#08ff3aff\">%s</TD>\n</TR>\n</TABLE> >];\n\n",
             node, node, node->object.name, node->father, left_line, right_line);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Рекурсивная функция создания связей между блоками
 * @param [in] node Указатель на текущий узел
 * @param [in] file Указатель на dot файл
 * @return 0
*/
// -------------------------------------------------------------------------------------------------------
int create_line (node_t* node,
                 FILE* file)
{
    assert (node);
    assert (file);

    if (node->left)
    {
        create_line (node->left, file);
    }

    if (node->right)
    {
        create_line (node->right, file);
    }

    if (node->right)
    {
        if (node->right->father == node)
        {
            fprintf (file,
                     "block_%p:right -> block_%p:root [color=\"#e501faff\", penwidth = 2, arrowsize = 0.6, constraint = true, dir = both];\n",
                     node, node->right);
        }

        else
        {
            fprintf (file,
                     "block_%p:right -> block_%p:root [color=\"#fa0101ff\", penwidth = 2, arrowsize = 0.6, constraint = true];\n",
                     node, node->right);
        }
    }

    if (node->left)
    {
        if (node->left->father == node)
        {
            fprintf (file,
                     "block_%p:left -> block_%p:root [color=\"#e501faff\", penwidth = 2, arrowsize = 0.6, constraint = true, dir = both];\n",
                     node, node->left);
        }

        else
        {
            fprintf (file,
                     "block_%p:left -> block_%p:root [color=\"#fa0101ff\", penwidth = 2, arrowsize = 0.6, constraint = true];\n",
                     node, node->left);
        }
    }
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
/**
 * @brief Функция создания html файла для debug
 * @param [in] root Указатель на родительский элемент
 * @param [in] reason Причина дампа
 * @param [in] add_string Дополнительная строка, которая выводится в dump
*/
// -------------------------------------------------------------------------------------------------------
int debug_html (node_t* root,
                const char* reason,
                const char* add_string)
{
    if (root == NULL)
    {
        return 0;
    }

    char comand[200] = "";
    sprintf (comand,
             "dot " ADDRESS_DUMP_DEBUG "graph.dot -T png -o " ADDRESS_DUMP_DEBUG "IMAGES/img%d.png",
             AMOUNT_IMAGES);
    int trash = system (comand);
    (void) trash;

    FILE* html_file = fopen (ADDRESS_DUMP_DEBUG "dump.html", "a");
    if (html_file == NULL)
    {
        EXIT_FUNC ("NULL file", -1);
    }

    fprintf (html_file,
             "Reason: %s\n",
             reason);

    if (add_string != NULL)
    {
        fprintf (html_file,
                 "Add string: %s\n",
                 add_string);
    }

    debug_graph (root);

    fprintf (html_file,
             "<img src=IMAGES/img%d.png width=100px>\n",
             AMOUNT_IMAGES);
    AMOUNT_IMAGES++;
    fprintf (html_file,
             "-----------------------------------------------------------------------------------------------\n");

    fclose (html_file);
    return 1;
}
// -------------------------------------------------------------------------------------------------------
