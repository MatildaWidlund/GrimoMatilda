#include "tree.h"
#include <stdbool.h>
#include "common.h"
#include "string.h"
#include <assert.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>

void test_tree_new()
{
    tree_t *new_tree = tree_new();
}
void test_tree_delete()
{
    tree_t *new_tree = tree_new();

}
void test_tree_size()
{
    tree_t *new_tree = tree_new();

}
void test_tree_depth()
{
    tree_t *new_tree = tree_new();

}
void test_tree_insert()
{
    tree_t *new_tree = tree_new();
    
    tree_key_t key1 {.i = 1};
    tree_key_t key1 {.i = 2}
    tree_key_t key1 {.i = 3}
    tree_key_t key1 {.i = 4}
    tree_key_t key1 {.i = 5}
    tree_key_t key1 {.i = 6}
    
    elem_t elem1 = {.i = 1};
    elem_t elem2 = {.i = 2};
    elem_t elem3 = {.i = 3};
    elem_t elem4 = {.i = 4};
    elem_t elem5 = {.i = 5};
    elem_t elem6 = {.i = 6};
    
    tree_insert(new_tree, key1, elem1);
    tree_insert(new_tree, key2, elem2);
    tree_insert(new_tree, key3, elem3);
    tree_insert(new_tree, key4, elem4);
    tree_insert(new_tree, key5, elem5);
    tree_insert(new_tree, key6, elem6);
    
    elem_t result = {.i = 0};
    
    tree_get(new_tree, 1, &result);
    int res1 = result.i;
    tree_get(new_tree, 2, &result);
    int res2 = result.i;
    tree_get(new_tree, 3, &result);
    int res3 = result.i;
    tree_get(new_tree, 4, &result);
    int res4 = result.i;
    tree_get(new_tree, 5, &result);
    int res5 = result.i;
    tree_get(new_tree, 6, &result);
    int res6 = result.i;
    
    CU_ASSERT_TRUE();
    CU_ASSERT_TRUE();
    CU_ASSERT_TRUE();
    CU_ASSERT_TRUE();
    CU_ASSERT_TRUE();
    CU_ASSERT_TRUE();

}
void test_tree_has_key()
{
    tree_t *new_tree = tree_new();

}
void test_tree_get()
{
    tree_t *new_tree = tree_new();

}
void test_tree_remove()
{
    tree_t *new_tree = tree_new();
}
void test_tree_keys()
{
    tree_t *new_tree = tree_new();

}
void test_tree_elements()
{
    tree_t *new_tree = tree_new();

}
void test_tree_apply()
{
    tree_t *new_tree = tree_new();

}








