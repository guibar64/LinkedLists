#include <stdio.h>

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "linked_list.h"

typedef LinkedListElement Element;

void test_elements() {
  LinkedListElement *element0 = linked_list_element_new(3);
  CU_ASSERT(element0->value == 3);
  linked_list_element_delete(element0);
}

void test_push() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(42));
  CU_ASSERT(list.head->value == 42);

  linked_list_push(&list, linked_list_element_new(66));
  CU_ASSERT(list.head->value == 42);
  CU_ASSERT(list.head->next->value == 66);

  linked_list_clean(&list);
}

void test_pop() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(42));
  linked_list_push(&list, linked_list_element_new(66));
  linked_list_push(&list, linked_list_element_new(-4));

  CU_ASSERT(linked_list_pop(&list) == -4);
  CU_ASSERT(linked_list_pop(&list) == 66);
  CU_ASSERT(linked_list_pop(&list) == 42);

  CU_ASSERT(list.head == NULL);

  linked_list_clean(&list);
}

void test_length() {
  LinkedList list = linked_list_new();
  CU_ASSERT(linked_list_length(&list) == 0);
  linked_list_push(&list, linked_list_element_new(42));
  CU_ASSERT(linked_list_length(&list) == 1);
  linked_list_push(&list, linked_list_element_new(66));
  CU_ASSERT(linked_list_length(&list) == 2);
  linked_list_push(&list, linked_list_element_new(-4));
  CU_ASSERT(linked_list_length(&list) == 3);
  linked_list_pop(&list);
  CU_ASSERT(linked_list_length(&list) == 2);

  linked_list_clean(&list);
}

void test_add() {
  LinkedList list = linked_list_new();
  Element *element0 = linked_list_element_new(42);
  linked_list_push(&list, element0);
  linked_list_push(&list, linked_list_element_new(-4));

  Element *element1 = linked_list_element_new(1);
  linked_list_add(&list, element1, 1);
  CU_ASSERT(list.head->next->value == 1);
  CU_ASSERT(list.head->next->next->value == -4);

  linked_list_add(&list, linked_list_element_new(0), 0);
  CU_ASSERT(list.head->value == 0);
  CU_ASSERT(list.head->next->value == 42);

  Element *not_added = linked_list_element_new(616);
  linked_list_add(&list, not_added, 10);
  CU_ASSERT(linked_list_length(&list) == 4);
  linked_list_element_delete(not_added);

  linked_list_clean(&list);
}

void test_add_after() {
  LinkedList list = linked_list_new();
  Element *zero, *one, *two;
  zero = linked_list_element_new(0);
  one = linked_list_element_new(1);
  two = linked_list_element_new(2);

  linked_list_push(&list, zero);
  linked_list_push(&list, one);
  linked_list_push(&list, two);

  linked_list_add_after(&list, linked_list_element_new(-1), one);
  CU_ASSERT(one->next->value == -1);
  CU_ASSERT(one->next->next->value == 2);

  linked_list_clean(&list);
}

void test_add_before() {
  LinkedList list = linked_list_new();
  Element *zero, *one, *two;
  zero = linked_list_element_new(0);
  one = linked_list_element_new(1);
  two = linked_list_element_new(2);
  linked_list_push(&list, zero);
  linked_list_push(&list, one);
  linked_list_push(&list, two);

  linked_list_add_before(&list, linked_list_element_new(-1), two);
  CU_ASSERT(one->next->value == -1);
  CU_ASSERT(one->next->next->value == 2);

  linked_list_add_before(&list, linked_list_element_new(-2), zero);
  CU_ASSERT(list.head->value == -2);
  CU_ASSERT(list.head->next->value == 0);

  linked_list_clean(&list);
}

int searchNeg(Value value, void *data) { return value < 0; }

int search_value(Value value, void *data) {
  Value *searched = (Value *)data;
  return value == *searched;
}

void test_search() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(0));
  linked_list_push(&list, linked_list_element_new(1));
  CU_ASSERT(linked_list_search(&list, searchNeg, NULL) == NULL);

  linked_list_push(&list, linked_list_element_new(-1));
  linked_list_push(&list, linked_list_element_new(2));
  CU_ASSERT(linked_list_search(&list, searchNeg, NULL)->value == -1);

  Value value = 2;
  CU_ASSERT(linked_list_search(&list, search_value, (void *)&value)->value ==
            2);

  linked_list_clean(&list);
}

void test_swap() {
  LinkedList list = linked_list_new();
  Element *elements[5];
  for (int i = 0; i < 5; i++) {
    elements[i] = linked_list_element_new(i);
    linked_list_push(&list, elements[i]);
  }
  linked_list_swap(&list, elements[2], elements[4]);
  // 0 1 4 3 2
  CU_ASSERT(elements[1]->next == elements[4]);
  CU_ASSERT(elements[4]->next == elements[3]);
  CU_ASSERT(elements[3]->next == elements[2]);
  CU_ASSERT(elements[2]->next == NULL);

  linked_list_swap(&list, elements[1], elements[0]);
  // 1 0 4 3 2
  CU_ASSERT(list.head == elements[1]);
  CU_ASSERT(elements[1]->next == elements[0]);

  linked_list_swap(&list, elements[0], elements[3]);
  // 1 3 4 0 2
  CU_ASSERT(list.head->next == elements[3]);
  CU_ASSERT(list.head->next->next->next == elements[0]);

  linked_list_clean(&list);
}

void test_revert() {
  LinkedList list = linked_list_new();
  Element *elements[5];
  for (int i = 0; i < 5; i++) {
    elements[i] = linked_list_element_new(i);
    linked_list_push(&list, elements[i]);
  }
  linked_list_revert(&list);
  size_t idx = 4;
  for (Element *elem_it = list.head; elem_it != NULL; elem_it = elem_it->next) {
    CU_ASSERT(elem_it == elements[idx]);
    idx--;
  }
  linked_list_clean(&list);
}

void test_remove_first() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(0));
  linked_list_push(&list, linked_list_element_new(1));
  linked_list_push(&list, linked_list_element_new(1));

  Value value = 1;
  linked_list_remove_first(&list, search_value, &value);
  CU_ASSERT(list.head->next->value == 1);

  linked_list_clean(&list);
}

void test_remove_all() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(0));
  linked_list_push(&list, linked_list_element_new(1));
  linked_list_push(&list, linked_list_element_new(1));
  linked_list_push(&list, linked_list_element_new(2));

  Value value = 1;
  linked_list_remove_all(&list, search_value, &value);
  CU_ASSERT(list.head->next->value == 2);

  value = 0;
  linked_list_remove_all(&list, search_value, &value);
  CU_ASSERT(list.head->value == 2);

  linked_list_clean(&list);
}

void test_remove_index() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(0));
  linked_list_push(&list, linked_list_element_new(1));
  linked_list_push(&list, linked_list_element_new(2));

  linked_list_remove_at_index(&list, 3);
  CU_ASSERT(list.head->next->next->value == 2);

  linked_list_remove_at_index(&list, 2);
  CU_ASSERT(list.head->next->next == NULL);

  linked_list_remove_at_index(&list, 1);
  CU_ASSERT(list.head->next == NULL);

  linked_list_remove_at_index(&list, 0);
  CU_ASSERT(list.head == NULL);
}

void test_print() {
  LinkedList list = linked_list_new();
  linked_list_push(&list, linked_list_element_new(0));
  linked_list_push(&list, linked_list_element_new(1));
  linked_list_push(&list, linked_list_element_new(2));

  print_linked_list(&list);
  linked_list_clean(&list);
}

void test_null_cases() {
  LinkedList list = linked_list_new();
  linked_list_revert(&list);
  CU_ASSERT(list.head == NULL);
  Element *elm1 = linked_list_element_new(-1);
  linked_list_add_after(&list, elm1, NULL);
  CU_ASSERT(list.head == NULL);

  linked_list_push(&list, linked_list_element_new(0));
  linked_list_swap(&list, NULL, NULL);
  CU_ASSERT(list.head->value == 0);
  linked_list_clean(&list);
  linked_list_element_delete(elm1);
}

void test_swap_on_head() {
  LinkedList list = linked_list_new();
  Element *el1 = linked_list_element_new(1);
  Element *el2 = linked_list_element_new(2);
  Element *el3 = linked_list_element_new(3);
  linked_list_push(&list, el1);
  linked_list_push(&list, el2);
  linked_list_push(&list, el3);

  linked_list_swap(&list, el1, el2);
  // 2 1 3
  CU_ASSERT(list.head == el2);
  CU_ASSERT(el2->next == el1);
  printf("el1 = %zd  el2 = %zd, el1->next = %zd", el1 - el1, el2 - el1,
         el1->next - el1);
  CU_ASSERT(el1->next == el3);

  linked_list_swap(&list, el1, el2);
  // 1 2 3
  CU_ASSERT(list.head == el1);

  print_linked_list(&list);

  linked_list_clean(&list);
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main(int argc, char *argv[]) {
  CU_pSuite pSuites = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  pSuites = CU_add_suite("linked_lists", init_suite, clean_suite);
  if (NULL == pSuites) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuites, "Elements", test_elements)) ||
      (NULL == CU_add_test(pSuites, "Push", test_push)) ||
      (NULL == CU_add_test(pSuites, "Pop", test_pop)) ||
      (NULL == CU_add_test(pSuites, "Length", test_length)) ||
      (NULL == CU_add_test(pSuites, "Add", test_add)) ||
      (NULL == CU_add_test(pSuites, "Before", test_add_before)) ||
      (NULL == CU_add_test(pSuites, "After", test_add_after)) ||
      (NULL == CU_add_test(pSuites, "Search", test_search)) ||
      (NULL == CU_add_test(pSuites, "Swap", test_swap)) ||
      (NULL == CU_add_test(pSuites, "Revert", test_revert)) ||
      (NULL == CU_add_test(pSuites, "Remove first", test_remove_first)) ||
      (NULL == CU_add_test(pSuites, "Remove all", test_remove_all)) ||
      (NULL == CU_add_test(pSuites, "Remove index", test_remove_index)) ||
      (NULL == CU_add_test(pSuites, "Print", test_print)) ||
      (NULL == CU_add_test(pSuites, "Null cases", test_null_cases)) ||
      (NULL == CU_add_test(pSuites, "Swap head", test_swap_on_head))) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (argc > 1) {
    while (--argc) {
      CU_pTest pTest;
      if ((pTest = CU_get_test_by_name(*(++argv), pSuites)) != NULL) {
        CU_basic_run_test(pSuites, pTest);
      } else {
        printf("WARNING: Unkown test: '%s'\n", *argv);
      }
    }
  } else {
    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
  }
  unsigned int fails = CU_get_number_of_failures();
  CU_cleanup_registry();
  if (fails > 0) {
    return 1;
  } else {
    return CU_get_error();
  }
}
