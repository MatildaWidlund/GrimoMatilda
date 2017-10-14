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

void test_list_length()
{ list_t *list = list_new();
    
    for(int i = 0; i <10; ++i) list_insert(list, i, NULL);
    CU_ASSERT_TRUE(list_length(list) == 10);
    
    for (int i = 0; i < 10; ++i) list_insert(list, i, NULL);
    CU_ASSERT_TRUE(list_length(list)==20);
    
    list remove(list, 0, NULL);
    CU_ASSERT_TRUE(list_length(list)==19);
    list_insert(list, 5, NULL);
    CU_ASSERT_TRUE(list_length(list)==20);
    list_remove(list, -1, NULL);
    list_remove(list, 1, NULL);
    CU_ASSERT_TRUE(list_length(list)== 18); //17?
    
    list_delete(list, list_action, true);
}


int main()
{
    //test_list_length
    return 0;
}
