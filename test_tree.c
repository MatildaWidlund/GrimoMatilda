#include "tree.h"
#include <stdbool.h>
#include "common.h"
#include "string.h"
#include <assert.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>


// TESTFUNCTIONS

void test_tree_new()
{
  tree_t *new_tree = tree_new(NULL, NULL, NULL, NULL);
    CU_ASSERT_TRUE(new_tree != NULL);
}

void test_tree_insert()
{
  tree_t *new_tree= tree_new(NULL,NULL, NULL, NULL);
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    tree_key_t key1 = { .i = 7};
    tree_key_t key2 = { .i = 8};
    tree_key_t key3 = { .i = 9};
    tree_key_t key4 = { .i = 10};
    tree_key_t key5 = { .i = 11};
    tree_key_t key6 = { .i = 12};



    
    tree_insert(new_tree, key1, elem1);
    tree_insert(new_tree, key2, elem2);
    tree_insert(new_tree, key3, elem3);
    tree_insert(new_tree, key4, elem4);
    tree_insert(new_tree, key5, elem5);
    tree_insert(new_tree, key6, elem6);
    
    elem_t result = { .i = 0};
    
    tree_get(new_tree, key1, &result);
    int res1 = result.i;
    tree_get(new_tree, key2, &result);
    int res2 = result.i;
    tree_get(new_tree, key3, &result);
    int res3 = result.i;
    tree_get(new_tree, key4, &result);
    int res4 = result.i;
    tree_get(new_tree, key5, &result);
    int res5 = result.i;
    
    CU_ASSERT_TRUE(res1 == 1);
    CU_ASSERT_TRUE(res2 == 2);
    CU_ASSERT_TRUE(res3 == 3);
    CU_ASSERT_TRUE(res4 == 4);
    CU_ASSERT_TRUE(res5 == 5);

}

void test_tree_size()
{
  tree_t *new_tree = tree_new(NULL, NULL, NULL, NULL);
    
    elem_t elem1 = { .i =1};
    elem_t elem2 = { .i =2};
    elem_t elem3 = { .i =3};
    elem_t elem4 = { .i =4};
    elem_t elem5 = { .i =5};
    elem_t elem6 = { .i =6};
    tree_key_t key1 = { .i = 7};
    tree_key_t key2 = { .i = 8};
    tree_key_t key3 = { .i = 9};
    tree_key_t key4 = { .i = 10};
    tree_key_t key5 = { .i = 11};
    tree_key_t key6 = { .i = 12};


    tree_insert(new_tree, key1, elem1);
    tree_insert(new_tree, key2, elem2);
    tree_insert(new_tree, key3, elem3);
    tree_insert(new_tree, key4, elem4);
    tree_insert(new_tree, key5, elem5);
    tree_insert(new_tree, key6, elem6);

    int size = tree_size(new_tree);
    CU_ASSERT_TRUE(size == 6);
    
    
}

int main()
{
    CU_initialize_registry();
    CU_pSuite tree = CU_add_suite("Test tree", NULL, NULL);
    CU_add_test(tree, "tree new", test_tree_new);
    CU_add_test(tree, "tree insert", test_tree_insert);
    CU_add_test(tree, "tree size", test_tree_size);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
