#ifndef LINKED_LIST_COMMON_HEADER
#define LINKED_LIST_COMMON_HEADER

#define TRUE 1
#define FALSE 0

typedef int Value;

/// @brief function prototype for search.
typedef int (*CriteriumFunction)(Value value, void *data);

#endif /*LINKED_LIST_COMMON_HEADER*/