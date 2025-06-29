//
// Created by giangvu on 3/14/25.
//

#ifndef LIST_COMMON_H
#define LIST_COMMON_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef enum {
    LIST_TYPE_UNKNOWN,
    LIST_TYPE_LIGHT,
    LIST_TYPE_COLOR,
    LIST_TYPE_GROUP,
    LIST_TYPE_FUNCTION
} ListType;


/*!
 * \brief Structure representing a node in the linked list.
 * This structure contains a pointer to generic data and pointers to the next and previous nodes.
 */
typedef struct NodeList{
    void * data;	/*!< information about the allocated block*/
    struct NodeList * next;	/*!< the address of the next element in the list*/
    struct NodeList * previous;	/*!< the address of the previous element in the list*/
}NodeList;

/*!
 * \brief Structure representing the linked list.
 * This structure contains pointers to the first, current, and last nodes in the list.
 */
typedef struct {
  NodeList sentinel_begin;
  NodeList* current;
  NodeList sentinel_end;
  ListType type;
} List;

/**
* @brief Allocates a list structure
* @return pointer to the newly created list
*/
List* allocList();

/*group 1: create a list and basic operations*/
/**
* @brief Initializes an empty list.
* Sets the first, current, and last pointers to NULL.
* @param l Pointer to the list to initialize.
* @param type point to the type of objects that the list contain
* @return 0 if success, -1 if error
*/
int initList(List * l, ListType type);

/**
* @brief Checks if the list is empty.
* @param l Pointer to the list to check.
* @return 1 if the list is empty, 0 otherwise.
*/
int isEmpty(const List * l);

/**
* @brief Inserts a new element at the beginning of the list.
* @param l Pointer to the list.
* @param data Pointer to the data to insert.
* @param dataType Type that the list contains
* @return 0 if fail and 1 if insert successfully
*/
int insertFirst(List *l, void *data, ListType dataType, int (*compareFunc)(void*, void*));

/**
* @brief Checks if the current element is the first element in the list.
* @param l Pointer to the list.
* @return 1 if the current element is the first, 0 otherwise.
*/
int isFirst(const List * l);

/**
* @brief Checks if the current element is the last element in the list.
* @param l Pointer to the list.
* @return 1 if the current element is the last, 0 otherwise.
*/
int isLast(const List * l);
/**
* @brief Checks if the current element is out of the list.
* @param l Pointer to the list.
* @return 1 if the current element is out of the list, 0 otherwise.
*/
int isOutOfList(const List *l);

/**
* @brief Gets the data of the current element.
* @param l Pointer to the list.
* @return A pointer to the data of the current element.
*/
void *getCurrentData(const List *l);

/**
* @brief Sets the current pointer to the first element in the list.
*
* @param l Pointer to the list.
*/
void setOnFirst(List *l);

/**
* @brief Sets the current pointer to the last element in the list.
* @param l Pointer to the list.
*/
void setOnLast(List *l);

/**
* @brief Sets the current pointer to the next element in the list.
* @param l Pointer to the list.
*/
void setOnNext(List *l);

/**
* @brief Sets the current pointer to the previous element in the list.
* @param l Pointer to the list.
*/
void setOnPrev(List *l);

/**
* @brief Prints the elements of the list using the provided print function.
* @param l Pointer to the list.
* @param printFunc Function pointer to a function that prints the data.
*/
void printList(const List *l, void (*printFunc)(void*));

/**
* @brief Gets the number of elements in the list.
*
* @param l Pointer to the list.
* @return The number of elements in the list.
*/
int nbElement(List *l);

/**
* @brief Return the smallest value as seen by the comparator
* @param l Pointer to the list.
* @param data Pointer to the data to find.
* @param data_found Output to the pointer to the data found.
* @param compareFunc Function pointer to a function that compares two data elements.
* @return The smallest value as seen by the comparator (0 if not succesful)
* @warning The pointer of the list will be at the last element
*/
int find_min(List *l, void *data, void *data_found, int (*compareFunc)(void*, void*));

/**
* @brief Finds an element in the list using the provided comparison function.
* @param l Pointer to the list.
* @param data Pointer to the data to find.
* @param compareFunc Function pointer to a function that compares two data elements.
* @return 1 if the element is found, 0 otherwise.
* @note The pointer will point to element found, otherwise pointing to the last element
*/
int find_exact(List *l, void *data, int (*compareFunc)(void*, void*));

/**
* @brief Deletes a specific element from the list.
* @param l Pointer to the list.
* @param data Pointer to the data to delete.
* @param compareFunc Function pointer to a function that compares two data elements.
* @return 1 if the element is successfully deleted, 0 otherwise.
*/
int deleteValue(List *l, void *data, int (*compareFunc)(void*, void*), void (*deleteFunc)( void*));

/**
* @brief Deletes all elements in the list and optionally frees the data.
* @param l Pointer to the list.
* @param deleteFunc Function used to free the data in each node (can be NULL).
*/
void deleteList(List *l, void (*deleteFunc)(void*));

/**
 * @brief Free function wrapper for to free allocated NodeList
 * **/
static void freeNodeList(NodeList * n);


#endif //LIST_COMMON_H
