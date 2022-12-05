#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include <stdlib.h>

#include "linked_list_common.h"

typedef struct DoublyLinkedListElement_ {
  Value value;
  struct DoublyLinkedListElement_ *next, *previous;
} DoublyLinkedListElement;

typedef struct DoublyLinkedList_ {
  DoublyLinkedListElement *head, *tail;
} DoublyLinkedList;

/** @brief creates an element **/
DoublyLinkedListElement *doubly_linked_list_element_new(Value value);

/// @brief delete an (isolated) element.
/// Can lead to double free if element is in list, prefer
/// doubly_linked_list_clean();
void doubly_linked_list_element_delete(DoublyLinkedListElement *element);

/// @brief delete an (isolated) element.
/// Can be unsafe if element is in list, prefer doubly_linked_list_clean();
DoublyLinkedList doubly_linked_list_new();

/// @brief add an element at the end of the list
void doubly_linked_list_push(DoublyLinkedList *list,
                             DoublyLinkedListElement *element);

/// @brief  remove the last element of the list -
/// @return last element value
Value doubly_linked_list_pop(DoublyLinkedList *list);

/// @brief
/// @return the length of the linked list
size_t doubly_linked_list_length();

/// add an element at an index
void doubly_linked_list_add(DoublyLinkedList *list,
                            DoublyLinkedListElement *element, size_t index);

/** @brief add an element after an element **/
void doubly_linked_list_add_after(DoublyLinkedList *list,
                                  DoublyLinkedListElement *element,
                                  DoublyLinkedListElement *after);

/// @brief  add an element before an element
void doubly_linked_list_add_before(DoublyLinkedList *list,
                                   DoublyLinkedListElement *element,
                                   DoublyLinkedListElement *before);

/// @brief  search  the first element based on function criterium
/// @param data  pointer to data that will be passed to criterium
/// @return NULL or the first element found
DoublyLinkedListElement *doubly_linked_list_search(DoublyLinkedList *list,
                                                   CriteriumFunction criterium,
                                                   void *data);

/// @brief  swap elements
void doubly_linked_list_swap(DoublyLinkedList *list,
                             DoublyLinkedListElement *element1,
                             DoublyLinkedListElement *element2);

/// @brief  revert all the element of the linked list
void doubly_linked_list_revert(DoublyLinkedList *list);

/// @brief  destroy the entire list
void doubly_linked_list_clean(DoublyLinkedList *list);

/// @brief  remove first element based on criterium
void doubly_linked_list_remove_first(DoublyLinkedList *list,
                                     CriteriumFunction criterium, void *data);

/// @brief  remove first element based on criterium
void doubly_linked_list_remove_all(DoublyLinkedList *list,
                                   CriteriumFunction criterium, void *data);

/// @brief  remove  element at index
void doubly_linked_list_remove_at_index(DoublyLinkedList *list, size_t index);

void print_doubly_linked_list(DoublyLinkedList *list);

void print_doubly_linked_list_reversed(DoublyLinkedList *list);

#endif /*LINKED_LIST_HEADER*/
