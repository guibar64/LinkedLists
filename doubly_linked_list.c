#include <stdio.h>

#include "doubly_linked_list.h"

typedef DoublyLinkedListElement Element;
typedef DoublyLinkedList LinkedList;

Element *doubly_linked_list_element_new(Value value) {
  Element retval = {value, NULL, NULL};
  Element *retptr = (Element *)malloc(sizeof(Element));
  *retptr = retval;
  return retptr;
}

void doubly_linked_list_element_delete(Element *element) { free(element); }

LinkedList doubly_linked_list_new() {
  LinkedList ret = {NULL, NULL};
  return ret;
}

void doubly_linked_list_push(LinkedList *list, Element *element) {
  if (list->head == NULL) {
    list->head = element;
    list->tail = element;
    return;
  }
  list->tail->next = element;
  element->previous = list->tail;
  list->tail = element;
}

Value doubly_linked_list_pop(LinkedList *list) {
  Value ret = 0;
  Element *last = list->tail;
  if (last != NULL) {
    if (last->previous != NULL) {
      last->previous->next = NULL;
    }
    list->tail = last->previous;
    if (list->head == last) {
      list->head = NULL;
    }
    ret = last->value;
    doubly_linked_list_element_delete(last);
  }
  return ret;
}

size_t doubly_linked_list_length(LinkedList *list) {
  size_t count = 0;
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    count++;
  }
  return count;
}

void doubly_linked_list_add(LinkedList *list, Element *element, size_t index) {
  size_t count = 0;
  Element *previous = NULL;
  Element *elem_it;
  for (elem_it = list->head; elem_it != NULL && count < index;
       elem_it = elem_it->next) {
    previous = elem_it;
    count++;
  }
  if (count != index)
    return;
  if (previous != NULL) {
    previous->next = element;
    if (element != NULL) {
      element->previous = previous;
    }
  }
  if (elem_it != NULL) {
    element->next = elem_it;
    elem_it->previous = element;
  }
  if (index == 0) {
    list->head = element;
    if (list->tail == NULL) {
      list->tail = element;
    }
  }
}

void doubly_linked_list_add_after(LinkedList *list, Element *element,
                                  Element *after) {
  if (after == NULL)
    return;
  if (element != NULL) {
    element->next = after->next;
    element->previous = after;
    if (after->next != NULL) {
      after->next->previous = element;
    }
  }
  after->next = element;
  if (list->tail == after) {
    list->tail = element;
  }
}

void doubly_linked_list_add_before(LinkedList *list, Element *element,
                                   Element *before) {
  if (before == NULL)
    return;
  if (element != NULL) {
    element->previous = before->previous;
    element->next = before;
    if (before->previous != NULL) {
      before->previous->next = element;
    }
  }
  before->previous = element;
  if (list->head == before) {
    list->head = element;
  }
}

Element *doubly_linked_list_search(LinkedList *list,
                                   CriteriumFunction criterium, void *data) {
  Element *found = NULL;
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    if (criterium(elem_it->value, data)) {
      found = elem_it;
      break;
    }
  }
  return found;
}

static void swap_close_elements(LinkedList *list, Element *element1,
                                Element *element2) {
  Element *next2 = element2->next;
  element2->next = element1;
  element2->previous = element1->previous;
  if (element1->previous != NULL) {
    element1->previous->next = element2;
  }

  element1->previous = element2;
  element1->next = next2;
  if (next2 != NULL) {
    next2->previous = element1;
  }
}

static void swap_far_elements(LinkedList *list, Element *element1,
                              Element *element2) {
  Element *previous2 = NULL, *next2 = NULL;

  previous2 = element2->previous;
  next2 = element2->next;

  if (element2->previous != NULL) {
    element2->previous->next = element1;
  }
  if (element2->next != NULL) {
    element2->next->previous = element1;
  }

  element2->previous = element1->previous;
  element2->next = element1->next;

  if (element1->previous != NULL) {
    element1->previous->next = element2;
  }
  if (element1->next != NULL) {
    element1->next->previous = element2;
  }

  element1->previous = previous2;
  element1->next = next2;
}

void doubly_linked_list_swap(LinkedList *list, Element *element1,
                             Element *element2) {
  if (element1 == element2 || element1 == NULL || element2 == NULL)
    return;

  if (element1 == list->head) {
    list->head = element2;
  } else if (element2 == list->head) {
    list->head = element1;
  }

  if (element1 == list->tail) {
    list->tail = element2;
  } else if (element2 == list->tail) {
    list->tail = element1;
  }

  if (element1->next == element2) {
    swap_close_elements(list, element1, element2);
  } else if (element2->next == element1) {
    swap_close_elements(list, element2, element1);
  } else {
    swap_far_elements(list, element1, element2);
  }
}

static void swap_element_pointers(Element **elem1, Element **elem2) {
  Element *tmp = *elem1;
  *elem1 = *elem2;
  *elem2 = tmp;
}

void doubly_linked_list_revert(LinkedList *list) {
  swap_element_pointers(&list->head, &list->tail);
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    swap_element_pointers(&elem_it->previous, &elem_it->next);
  }
}

void doubly_linked_list_clean(LinkedList *list) {
  Element *elem_it = list->head;
  while (elem_it != NULL) {
    Element *element = elem_it;
    elem_it = elem_it->next;
    doubly_linked_list_element_delete(element);
  }
}

static void remove_element(LinkedList *list, Element *element) {

  if (element == NULL)
    return;
  Element *previous = element->previous;
  if (previous != NULL) {
    previous->next = element->next;
  }
  if (element->next != NULL) {
    element->next->previous = previous;
  }
  if (list->head == element) {
    list->head = element->next;
  }
  if (list->tail == element) {
    list->tail = element->previous;
  }
  doubly_linked_list_element_delete(element);
}

void doubly_linked_list_remove_first(LinkedList *list,
                                     CriteriumFunction criterium, void *data) {
  Element *elem_it;
  for (elem_it = list->head; elem_it != NULL; elem_it = elem_it->next) {
    if (criterium(elem_it->value, data)) {
      remove_element(list, elem_it);
      return;
    }
  }
}

void doubly_linked_list_remove_all(LinkedList *list,
                                   CriteriumFunction criterium, void *data) {
  Element *previous = NULL;
  Element *elem_it;
  for (elem_it = list->head; elem_it != NULL; elem_it = elem_it->next) {
    if (criterium(elem_it->value, data)) {
      remove_element(list, elem_it);
      // will point to removed element's next at next iteration.
      if (previous == NULL) {
        elem_it = list->head;
      } else {
        elem_it = previous;
      }
    }
    previous = elem_it;
  }
}

void doubly_linked_list_remove_at_index(LinkedList *list, size_t index) {
  Element *elem_it;
  size_t count = 0;
  for (elem_it = list->head; elem_it != NULL && count < index;
       elem_it = elem_it->next) {
    count++;
  }
  if (count == index) {
    remove_element(list, elem_it);
  }
}

void print_doubly_linked_list(LinkedList *list) {
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    printf("%d ", elem_it->value);
  }

  printf("\n");
}

void print_doubly_linked_list_reversed(LinkedList *list) {
  for (Element *elem_it = list->tail; elem_it != NULL;
       elem_it = elem_it->previous) {
    printf("%d ", elem_it->value);
  }
  printf("\n");
}
