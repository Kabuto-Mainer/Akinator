#include <stdio.h>

#include "func.h"

int main (void)
{
    tree_t tree = {};
    create_tree (&tree);

    for (int i = 0; i < 3; i++)
    {
        guess_object (&tree);
        dump_tree (&tree, "test");
    }

    save_data (&tree);

    // desc_object (&tree);
    // compare_objects (&tree);

    delete_tree (&tree);

    return 0;
}
