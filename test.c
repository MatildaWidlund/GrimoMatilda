#include "list.h"
#include <stdbool.h>
#include "common.h"
#include "string.h"

typedef struct item{
char* name;
}item_p;


void free_function(elem_t free)
{
    return;
}

int comp_function(elem_t a, elem_t b)
{
    elem_t *test = a.p;
    elem_t *test2 = b.p;
    int comp = strcmp(((item_p*)test)->name, ((item_p*)test2)->name);
    if (comp > 0){
        return 2;
    }
    if (comp <0 )
    {
            return 1;
        }
    else{
        return 0;
    }
}

elem_t copy_function (elem_t copy)
{
    return copy;
}

int main()
{
  list_t *list = list_new(copy_function, free_function, comp_function);
    return 0;
}
