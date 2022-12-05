#include <stdio.h>

#include "linked_list.h"

typedef LinkedListElement Element;

LinkedListElement *linked_list_element_new(Value value) {
  Element retval = {value, NULL};
  Element *retptr = (Element *)malloc(sizeof(Element));
  *retptr = retval;
  return retptr;
}

void linked_list_element_delete(LinkedListElement *element) { free(element); }

LinkedList linked_list_new() {
  LinkedList ret = {NULL};
  return ret;
}

void linked_list_push(LinkedList *list, LinkedListElement *element) {
  Element *elem_it = list->head;
  if (elem_it == NULL) {
    list->head = element;
    return;
  }
  while (elem_it->next != NULL) {
    elem_it = elem_it->next;
  }
  elem_it->next = element;
}

Value linked_list_pop(LinkedList *list) {
  Element *last = NULL, *previous_last = NULL;
  Element *elem_it = list->head;
  while (elem_it != NULL) {
    previous_last = last;
    last = elem_it;
    elem_it = elem_it->next;
  }
  Value ret = last->value;
  if (previous_last != NULL) {
    previous_last->next = NULL;
  }
  if (list->head == last) {
    list->head = NULL;
  }
  linked_list_element_delete(last);
  return ret;
}

size_t linked_list_length(LinkedList *list) {
  size_t count = 0;
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    count++;
  }
  return count;
}

void linked_list_add(LinkedList *list, Element *element, size_t index) {
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
  }
  if (elem_it != NULL) {
    element->next = elem_it;
  }
  if (index == 0) {
    list->head = element;
  }
}

void linked_list_add_after(LinkedList *list, LinkedListElement *element,
                           LinkedListElement *after) {
  if (after == NULL)
    return;
  if (element != NULL) {
    element->next = after->next;
  }
  after->next = element;
}

void linked_list_add_before(LinkedList *list, LinkedListElement *element,
                            LinkedListElement *before) {
  Element *previous = NULL;
  for (Element *elem_it = list->head; elem_it != NULL && elem_it != before;
       elem_it = elem_it->next) {
    previous = elem_it;
  }
  if (element != NULL) {
    element->next = before;
  }
  if (previous != NULL) {
    previous->next = element;
  }
  if (list->head == before) {
    list->head = element;
  }
}

LinkedListElement *linked_list_search(LinkedList *list,
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

void linked_list_swap(LinkedList *list, LinkedListElement *element1,
                      LinkedListElement *element2) {
  Element *previous1 = NULL, *previous2 = NULL;
  if (element1 == element2 || element1 == NULL || element2 == NULL)
    return;
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    if (elem_it->next == element1) {
      previous1 = elem_it;
    }
    if (elem_it->next == element2) {
      previous2 = elem_it;
    }
  }
  Element *element2_next = element2->next;
  if (element1->next == element2) {
    element2->next = element1;
  } else {
    element2->next = element1->next;
    if (previous2 != NULL) {
      previous2->next = element1;
    }
  }

  if (element2_next == element1) {
    element1->next = element2;
  } else {
    element1->next = element2_next;
    if (previous1 != NULL) {
      previous1->next = element2;
    }
  }
  if (element1 == list->head) {
    list->head = element2;
  } else if (element2 == list->head) {
    list->head = element1;
  }
}

static Element **collect_elements(LinkedList *list, size_t *length) {
  *length = linked_list_length(list);
  if (*length == 0)
    return NULL;
  Element **ret = (Element **)malloc(*length * sizeof(Element *));
  size_t idx = 0;
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    ret[idx++] = elem_it;
  }
  return ret;
}

void linked_list_revert(LinkedList *list) {
  size_t length;
  Element **elements = collect_elements(list, &length);
  if (length == 0)
    return;
  list->head = elements[length - 1];
  for (size_t i = 1; i < length; i++) {
    size_t idx = length - 1 - i;
    elements[idx + 1]->next = elements[idx];
  }
  elements[0]->next = NULL;
  free(elements);
}

void linked_list_clean(LinkedList *list) {
  Element *elem_it = list->head;
  while (elem_it != NULL) {
    Element *element = elem_it;
    elem_it = elem_it->next;
    linked_list_element_delete(element);
  }
}

static void remove_element(LinkedList *list, Element *element,
                           Element *previous) {
  if (element == NULL)
    return;
  if (previous != NULL) {
    previous->next = element->next;
  }
  if (list->head == element) {
    list->head = element->next;
  }
  linked_list_element_delete(element);
}

void linked_list_remove_first(LinkedList *list, CriteriumFunction criterium,
                              void *data) {
  Element *previous = NULL;
  Element *elem_it;
  for (elem_it = list->head; elem_it != NULL; elem_it = elem_it->next) {
    if (criterium(elem_it->value, data)) {
      remove_element(list, elem_it, previous);
      return;
    }
    previous = elem_it;
  }
}

void linked_list_remove_all(LinkedList *list, CriteriumFunction criterium,
                            void *data) {
  Element *previous = NULL;
  Element *elem_it;
  for (elem_it = list->head; elem_it != NULL; elem_it = elem_it->next) {
    if (criterium(elem_it->value, data)) {
      remove_element(list, elem_it, previous);
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

void linked_list_remove_at_index(LinkedList *list, size_t index) {
  Element *previous = NULL;
  Element *elem_it;
  size_t count = 0;
  for (elem_it = list->head; elem_it != NULL && count < index;
       elem_it = elem_it->next) {
    previous = elem_it;
    count++;
  }
  if (count == index) {
    remove_element(list, elem_it, previous);
  }
}

void print_linked_list(LinkedList *list) {
  for (Element *elem_it = list->head; elem_it != NULL;
       elem_it = elem_it->next) {
    printf("%d ", elem_it->value);
  }
  printf("\n");
}
