#include "list.h"
#include <stdbool.h>
#include "common.h"
#include "string.h"
#include <assert.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>


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


// TESTFUNCTIONS

void test_list_new()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    CU_ASSERT_TRUE(new_list != NULL);
}

void test_list_insert()
{
    list_t *new_list= list_new(NULL,NULL, NULL);
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    
    list_insert(new_list, 0, elem1);
    list_insert(new_list, 1, elem2);
    list_insert(new_list, 2, elem3);
    list_insert(new_list, 3, elem4);
    list_insert(new_list, 4, elem5);
    list_insert(new_list, 5, elem6);
    
    elem_t result = { .i = 0};
    
    list_first(new_list, &result);
    int res1 = result.i;
    list_get(new_list, 1, &result);
    int res2 = result.i;
    list_get(new_list, 2, &result);
    int res3 = result.i;
    list_get(new_list, 3, &result);
    int res4 = result.i;
    list_get(new_list, 4, &result);
    int res5 = result.i;
    list_last(new_list, &result);
    int res6 = result.i;
    
    CU_ASSERT_TRUE(res1 == 1);
    CU_ASSERT_TRUE(res2 == 2);
    CU_ASSERT_TRUE(res3 == 3);
    CU_ASSERT_TRUE(res4 == 4);
    CU_ASSERT_TRUE(res5 == 5);
    CU_ASSERT_TRUE(res6 == 6);

}

void test_list_prepend()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    
    elem_t result = { .i =0};
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    
    list_prepend(new_list, elem6);
    list_prepend(new_list, elem5);
    list_prepend(new_list, elem4);
    list_prepend(new_list, elem3);
    list_prepend(new_list, elem2);
    list_prepend(new_list, elem1);
    
    list_first(new_list, &result);
    int res1 = result.i;
    list_get(new_list, 1, &result);
    int res2 =result.i;
    list_get(new_list, 2, &result);
    int res3 =result.i;
    list_get(new_list, 3, &result);
    int res4 = result.i;
    list_get(new_list, 4, &result);
    int res5 = result.i;
    list_last(new_list, &result);
    int res6 = result.i;
    
    CU_ASSERT_TRUE(res1 == 1);
    CU_ASSERT_TRUE(res2 == 2);
    CU_ASSERT_TRUE(res3 == 3);
    CU_ASSERT_TRUE(res4 == 4);
    CU_ASSERT_TRUE(res5 == 5);
    CU_ASSERT_TRUE(res6 == 6);
    
}

void test_list_prepend_empty()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    elem_t result = { .i = 0};
    elem_t elem1 = { .i = 1};
    
    list_prepend(new_list, elem1);
    list_first(new_list, &result);
    
    int res1 =result.i;
    
    CU_ASSERT_TRUE(res1 == 1);
    
}

void test_list_append()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    elem_t result = { .i = 0};
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    
    list_append(new_list, elem1);
    list_append(new_list, elem2);
    list_append(new_list, elem3);
    list_append(new_list, elem4);
    list_append(new_list, elem5);
    list_append(new_list, elem6);
    
    list_first(new_list, &result);
    int res1 = result.i;
    list_get(new_list, 1, &result);
    int res2 =result.i;
    list_get(new_list, 2, &result);
    int res3 =result.i;
    list_get(new_list, 3, &result);
    int res4 = result.i;
    list_get(new_list, 4, &result);
    int res5 = result.i;
    list_last(new_list, &result);
    int res6 = result.i;
    
    CU_ASSERT_TRUE(res1 == 1);
    CU_ASSERT_TRUE(res2 == 2);
    CU_ASSERT_TRUE(res3 == 3);
    CU_ASSERT_TRUE(res4 == 4);
    CU_ASSERT_TRUE(res5 == 5);
    CU_ASSERT_TRUE(res6 == 6);
    
}

void test_list_append_empty()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    elem_t result = { .i = 0};
    elem_t elem1 = { .i = 1};
    elem_t elem2 = { .i = 2};
    
    list_append(new_list, elem1);
    list_append(new_list, elem2);
    
    list_first(new_list, &result);
    int res1 = result.i;
    list_last(new_list, &result);
    int res2 = result.i;
    
    CU_ASSERT_TRUE(res1 == 1);
    CU_ASSERT_TRUE(res2 == 2);
}

void test_list_length()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    
    list_append(new_list, elem1);
    list_append(new_list, elem2);
    list_append(new_list, elem3);
    list_append(new_list, elem4);
    list_append(new_list, elem5);
    list_append(new_list, elem6);
    
    int length = list_length(new_list);
    CU_ASSERT_TRUE(length == 6);

}

void test_list_length_empty()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    int length = list_length(new_list);
    
    CU_ASSERT_TRUE(length == 0);
}

void test_list_remove_last_item()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    
    elem_t elem1 ={ .i = 1};
    
    list_append(new_list, elem1);
    list_remove(new_list, 0, false);
    int length = list_length(new_list);
    CU_ASSERT_TRUE(length == 0);
}

void test_list_remove_item()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    
    list_append(new_list, elem1);
    list_append(new_list, elem2);
    list_append(new_list, elem3);
    list_append(new_list, elem4);
    list_append(new_list, elem5);
    list_append(new_list, elem6);
    
    list_remove(new_list, 5, false);
    int length = list_length(new_list);
    CU_ASSERT_TRUE(length ==5);
    /* 
    list_remove(new_list, 4, false);
    length = list_length(new_list);
    CU_ASSERT_TRUE(length ==4);
    
    list_remove(new_list, 3, false);
    length = list_length(new_list);
    CU_ASSERT_TRUE(length == 3);
    
    list_remove(new_list, 2, false);
    length = list_length(new_list);
    CU_ASSERT_TRUE(length == 2);
    
    list_remove(new_list, 1, false);
    length = list_length(new_list);
    CU_ASSERT_TRUE(length ==1);
    
    list_remove(new_list, 0, false);
    length = list_length(new_list);
    CU_ASSERT_TRUE(length == 0);*/
}

void test_list_contains()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    
    list_append(new_list, elem1);
    list_append(new_list, elem2);
    list_append(new_list, elem3);
    list_append(new_list, elem4);
    list_append(new_list, elem5);
    list_append(new_list, elem6);
    
    int result = list_contains(new_list, elem5);
    CU_ASSERT_TRUE(result ==4);
}

void test_list_contains_empty()
{
    list_t *new_list = list_new(NULL, NULL, NULL);
    
    elem_t elem1 = { .i = 1};
    int index = list_contains(new_list, elem1);
    
    CU_ASSERT_TRUE(index == -1);
}
/*
void test_list_length()
{ list_t *list = list_new(NULL, NULL, NULL);
    
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
*/

int main()
{
    CU_initialize_registry();
    CU_pSuite list = CU_add_suite("Test list", NULL, NULL);
    CU_add_test(list, "list new", test_list_new);
    CU_add_test(list, "list insert", test_list_insert);
    CU_add_test(list, "list prepend", test_list_prepend);
    CU_add_test(list, "list prepend empty", test_list_prepend_empty);
    CU_add_test(list, "list append", test_list_append);
    CU_add_test(list, "list append empty", test_list_append_empty);
    CU_add_test(list, "list length", test_list_length);
    CU_add_test(list, "list length empty", test_list_length_empty);
    CU_add_test(list, "list remove last item", test_list_remove_last_item);
    CU_add_test(list, "list remove item", test_list_remove_item);
    CU_add_test(list, "list contains", test_list_contains);
    CU_add_test(list, "list contains is empty", test_list_contains_empty);
    
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
