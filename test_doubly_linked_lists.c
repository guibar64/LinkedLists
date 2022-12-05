#include <stdio.h>

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "doubly_linked_list.h"

typedef DoublyLinkedListElement Element;
typedef DoublyLinkedList LinkedList;

void test_elements() {
  Element *element0 = doubly_linked_list_element_new(3);
  CU_ASSERT(element0->value == 3);
  doubly_linked_list_element_delete(element0);
}

void test_push() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(42));
  CU_ASSERT(list.head->value == 42);
  CU_ASSERT(list.tail->value == 42);

  doubly_linked_list_push(&list, doubly_linked_list_element_new(66));
  CU_ASSERT(list.head->value == 42);
  CU_ASSERT(list.head->next->value == 66);
  CU_ASSERT(list.tail->previous->value == 42);

  doubly_linked_list_clean(&list);
}

void test_pop() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(42));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(66));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(-4));

  CU_ASSERT(doubly_linked_list_pop(&list) == -4);
  CU_ASSERT(list.tail->value == 66);
  CU_ASSERT(doubly_linked_list_pop(&list) == 66);
  CU_ASSERT(list.tail->value == 42);
  CU_ASSERT(doubly_linked_list_pop(&list) == 42);

  CU_ASSERT(list.head == NULL);
  CU_ASSERT(list.tail == NULL);

  doubly_linked_list_clean(&list);
}

void test_length() {
  LinkedList list = doubly_linked_list_new();
  CU_ASSERT(doubly_linked_list_length(&list) == 0);
  doubly_linked_list_push(&list, doubly_linked_list_element_new(42));
  CU_ASSERT(doubly_linked_list_length(&list) == 1);
  doubly_linked_list_push(&list, doubly_linked_list_element_new(66));
  CU_ASSERT(doubly_linked_list_length(&list) == 2);
  doubly_linked_list_push(&list, doubly_linked_list_element_new(-4));
  CU_ASSERT(doubly_linked_list_length(&list) == 3);
  doubly_linked_list_pop(&list);
  CU_ASSERT(doubly_linked_list_length(&list) == 2);

  doubly_linked_list_clean(&list);
}

void test_add() {
  LinkedList list = doubly_linked_list_new();
  Element *element0 = doubly_linked_list_element_new(42);
  doubly_linked_list_push(&list, element0);
  doubly_linked_list_push(&list, doubly_linked_list_element_new(-4));

  Element *element1 = doubly_linked_list_element_new(1);
  doubly_linked_list_add(&list, element1, 1);
  CU_ASSERT(list.head->next->value == 1);
  CU_ASSERT(list.head->next->next->value == -4);
  CU_ASSERT(list.tail->previous->value == 1);

  doubly_linked_list_add(&list, doubly_linked_list_element_new(0), 0);
  CU_ASSERT(list.head->value == 0);
  CU_ASSERT(list.tail->value == -4);
  CU_ASSERT(list.head->next->value == 42);
  CU_ASSERT(list.head->next->previous == list.head);

  Element *not_added = doubly_linked_list_element_new(616);
  doubly_linked_list_add(&list, not_added, 10);
  CU_ASSERT(doubly_linked_list_length(&list) == 4);
  doubly_linked_list_element_delete(not_added);

  doubly_linked_list_clean(&list);
}

void test_add_after() {
  LinkedList list = doubly_linked_list_new();
  Element *zero, *one, *two;
  zero = doubly_linked_list_element_new(0);
  one = doubly_linked_list_element_new(1);
  two = doubly_linked_list_element_new(2);

  doubly_linked_list_push(&list, zero);
  doubly_linked_list_push(&list, one);
  doubly_linked_list_push(&list, two);

  doubly_linked_list_add_after(&list, doubly_linked_list_element_new(-1), one);
  CU_ASSERT(one->next->value == -1);
  CU_ASSERT(one->next->previous == one);
  CU_ASSERT(one->next->next == two);
  CU_ASSERT(two->previous->previous == one);

  doubly_linked_list_clean(&list);
}

void test_add_before() {
  LinkedList list = doubly_linked_list_new();
  Element *zero, *one, *two;
  zero = doubly_linked_list_element_new(0);
  one = doubly_linked_list_element_new(1);
  two = doubly_linked_list_element_new(2);
  doubly_linked_list_push(&list, zero);
  doubly_linked_list_push(&list, one);
  doubly_linked_list_push(&list, two);

  doubly_linked_list_add_before(&list, doubly_linked_list_element_new(-1), two);
  CU_ASSERT(one->next->value == -1);
  CU_ASSERT(one->next->next->value == 2);
  CU_ASSERT(two->previous->value == -1);
  CU_ASSERT(two->previous->previous == one);

  doubly_linked_list_add_before(&list, doubly_linked_list_element_new(-2),
                                zero);
  CU_ASSERT(list.head->value == -2);
  CU_ASSERT(zero->previous->value == -2);
  CU_ASSERT(list.head->next->value == 0);

  doubly_linked_list_clean(&list);
}

int searchNeg(Value value, void *data) { return value < 0; }

int search_value(Value value, void *data) {
  Value *searched = (Value *)data;
  return value == *searched;
}

void test_search() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(0));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));
  CU_ASSERT(doubly_linked_list_search(&list, searchNeg, NULL) == NULL);

  doubly_linked_list_push(&list, doubly_linked_list_element_new(-1));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(2));
  CU_ASSERT(doubly_linked_list_search(&list, searchNeg, NULL)->value == -1);

  Value value = 2;
  CU_ASSERT(
      doubly_linked_list_search(&list, search_value, (void *)&value)->value ==
      2);

  doubly_linked_list_clean(&list);
}

void test_swap() {
  LinkedList list = doubly_linked_list_new();
  Element *elements[5];
  for (int i = 0; i < 5; i++) {
    elements[i] = doubly_linked_list_element_new(i);
    doubly_linked_list_push(&list, elements[i]);
  }
  doubly_linked_list_swap(&list, elements[2], elements[4]);
  // 0 1 4 3 2
  CU_ASSERT(elements[1]->next == elements[4]);
  CU_ASSERT(elements[4]->next == elements[3]);
  CU_ASSERT(elements[3]->next == elements[2]);
  CU_ASSERT(elements[2]->next == NULL);

  // doubly_linked_list_swap(&list, elements[1], elements[0]);
  doubly_linked_list_swap(&list, elements[0], elements[1]);
  // 1 0 4 3 2
  CU_ASSERT(list.head == elements[1]);
  CU_ASSERT(elements[1]->next == elements[0]);

  doubly_linked_list_swap(&list, elements[0], elements[3]);
  // 1 3 4 0 2
  CU_ASSERT(list.head->next == elements[3]);
  CU_ASSERT(list.head->next->next->next == elements[0]);

  doubly_linked_list_clean(&list);
}

void test_swap2() {
  LinkedList list = doubly_linked_list_new();
  Element *elements[5];
  for (int i = 0; i < 5; i++) {
    elements[i] = doubly_linked_list_element_new(i);
    doubly_linked_list_push(&list, elements[i]);
  }
  doubly_linked_list_swap(&list, elements[2], elements[3]);
  // 0 1 3 2 4
  CU_ASSERT(list.head->next->next == elements[3]);
  CU_ASSERT(list.tail->previous == elements[2]);

  doubly_linked_list_swap(&list, elements[2], elements[3]);
  // 0 1 2 3 4
  CU_ASSERT(list.head->next->next == elements[2]);
  CU_ASSERT(list.tail->previous == elements[3]);

  doubly_linked_list_clean(&list);
}

void test_revert() {
  LinkedList list = doubly_linked_list_new();
  Element *elements[5];
  for (int i = 0; i < 5; i++) {
    elements[i] = doubly_linked_list_element_new(i);
    doubly_linked_list_push(&list, elements[i]);
  }
  doubly_linked_list_revert(&list);
  size_t idx = 4;
  for (Element *elem_it = list.head; elem_it != NULL; elem_it = elem_it->next) {
    CU_ASSERT(elem_it == elements[idx]);
    idx--;
  }
  doubly_linked_list_clean(&list);
}

void test_remove_first() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(0));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));

  Value value = 1;
  doubly_linked_list_remove_first(&list, search_value, &value);
  CU_ASSERT(list.head->next->value == 1);

  doubly_linked_list_clean(&list);
}

void test_remove_all() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(0));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(2));

  Value value = 1;
  doubly_linked_list_remove_all(&list, search_value, &value);
  CU_ASSERT(list.head->next->value == 2);

  value = 0;
  doubly_linked_list_remove_all(&list, search_value, &value);
  CU_ASSERT(list.head->value == 2);

  doubly_linked_list_clean(&list);
}

void test_remove_index() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(0));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(2));

  doubly_linked_list_remove_at_index(&list, 3);
  CU_ASSERT(list.head->next->next->value == 2);

  doubly_linked_list_remove_at_index(&list, 2);
  CU_ASSERT(list.head->next->next == NULL);

  doubly_linked_list_remove_at_index(&list, 1);
  CU_ASSERT(list.head->next == NULL);

  doubly_linked_list_remove_at_index(&list, 0);
  CU_ASSERT(list.head == NULL);
}

void test_print() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_push(&list, doubly_linked_list_element_new(0));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(1));
  doubly_linked_list_push(&list, doubly_linked_list_element_new(2));

  print_doubly_linked_list(&list);
  print_doubly_linked_list_reversed(&list);

  doubly_linked_list_clean(&list);
}

void test_null_cases() {
  LinkedList list = doubly_linked_list_new();
  doubly_linked_list_revert(&list);
  CU_ASSERT(list.head == NULL);
  Element *elm1 = doubly_linked_list_element_new(-1);
  doubly_linked_list_add_after(&list, elm1, NULL);
  CU_ASSERT(list.head == NULL);

  doubly_linked_list_push(&list, doubly_linked_list_element_new(0));
  doubly_linked_list_swap(&list, NULL, NULL);
  CU_ASSERT(list.head->value == 0);
  doubly_linked_list_clean(&list);
  doubly_linked_list_element_delete(elm1);

  list = doubly_linked_list_new();
  doubly_linked_list_clean(&list);
  doubly_linked_list_add(&list, doubly_linked_list_element_new(0), 0);
  CU_ASSERT(list.tail->value == 0);

  doubly_linked_list_clean(&list);
}

void test_swap_on_head() {
  LinkedList list = doubly_linked_list_new();
  Element *el1 = doubly_linked_list_element_new(1);
  Element *el2 = doubly_linked_list_element_new(2);
  doubly_linked_list_push(&list, el1);
  doubly_linked_list_push(&list, el2);

  doubly_linked_list_swap(&list, el1, el2);
  CU_ASSERT(list.head == el2);
  CU_ASSERT(list.tail == el1);
  doubly_linked_list_swap(&list, el1, el2);
  CU_ASSERT(list.head == el1);
  CU_ASSERT(list.tail == el2);

  doubly_linked_list_clean(&list);
}

void test_addd_after_before_2() {
  LinkedList list = doubly_linked_list_new();
  Element *el1 = doubly_linked_list_element_new(1);
  Element *el2 = doubly_linked_list_element_new(2);
  doubly_linked_list_push(&list, el1);
  doubly_linked_list_push(&list, el2);

  Element *el3 = doubly_linked_list_element_new(3);
  doubly_linked_list_add_before(&list, el3, NULL);
  doubly_linked_list_add_after(&list, el3, list.tail);
  CU_ASSERT(list.tail == el3);

  // doubly_linked_list_element_delete(el3);

  doubly_linked_list_clean(&list);
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
      (NULL == CU_add_test(pSuites, "Swap 2", test_swap2)) ||
      (NULL == CU_add_test(pSuites, "Print", test_print)) ||
      (NULL == CU_add_test(pSuites, "Swap 3", test_swap_on_head)) ||
      (NULL == CU_add_test(pSuites, "Null cases", test_null_cases)) ||
      (NULL ==
       CU_add_test(pSuites, "Add Before After 2", test_addd_after_before_2))) {
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
