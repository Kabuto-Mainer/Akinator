#include <stdio.h>

#include "func.h"

int main (void)
{
    tree_t tree = {};
    create_tree (&tree);

    for (int i = 0; i < 10; i++)
    {
        guess_object (&tree);
        dump_tree (&tree, "test");
    }

    delete_tree (&tree);

    return 0;
}
