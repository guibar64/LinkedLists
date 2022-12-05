#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include <stdlib.h>

#include "linked_list_common.h"

typedef struct LinkedListElement_ {
  Value value;
  struct LinkedListElement_ *next;
} LinkedListElement;

typedef struct LinkedList_ {
  LinkedListElement *head;
} LinkedList;

/** @brief creates an element **/
LinkedListElement *linked_list_element_new(Value value);

/// @brief delete an (isolated) element.
/// Can lead to double free if element is in list, prefer linked_list_clean();
void linked_list_element_delete(LinkedListElement *element);

/// @brief delete an (isolated) element.
/// Can be unsafe if element is in list, prefer linked_list_clean();
LinkedList linked_list_new();

/// @brief add an element at the end of the list
void linked_list_push(LinkedList *list, LinkedListElement *element);

/// @brief  remove the last element of the list -
/// @return last element value
Value linked_list_pop(LinkedList *list);

/// @brief
/// @return the length of the linked list
size_t linked_list_length();

/// add an element at an index
void linked_list_add(LinkedList *list, LinkedListElement *element,
                     size_t index);

/** @brief add an element after an element **/
void linked_list_add_after(LinkedList *list, LinkedListElement *element,
                           LinkedListElement *after);

/// @brief  add an element before an element
void linked_list_add_before(LinkedList *list, LinkedListElement *element,
                            LinkedListElement *before);

/// @brief  search  the first element based on function criterium
/// @param data  pointer to data that will be passed to criterium
/// @return NULL or the first element found
LinkedListElement *linked_list_search(LinkedList *list,
                                      CriteriumFunction criterium, void *data);

/// @brief  swap elements
void linked_list_swap(LinkedList *list, LinkedListElement *element1,
                      LinkedListElement *element2);

/// @brief  revert all the element of the linked list
void linked_list_revert(LinkedList *list);

/// @brief  destroy the entire list
void linked_list_clean(LinkedList *list);

/// @brief  remove first element based on criterium
void linked_list_remove_first(LinkedList *list, CriteriumFunction criterium,
                              void *data);

/// @brief  remove first element based on criterium
void linked_list_remove_all(LinkedList *list, CriteriumFunction criterium,
                            void *data);

/// @brief  remove  element at index
void linked_list_remove_at_index(LinkedList *list, size_t index);

void print_linked_list(LinkedList *list);

#endif /*LINKED_LIST_HEADER*/
