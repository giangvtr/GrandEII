#include "list_common.h"

List* allocList() {
    List* newList = NULL;
    newList = (List*)malloc(sizeof(List));
    if (newList == NULL) {
        return NULL;
    }
    return newList;
}

int initList(List *l,ListType type) {
    if(l != NULL) {
        l->sentinel_begin.next = &(l->sentinel_end);
        l->sentinel_begin.previous = NULL;
        l->sentinel_end.previous = &(l->sentinel_begin);
        l->sentinel_end.next = NULL;
        l->current = &(l->sentinel_begin);
        l->type = type;
        return 0;
    }
    return -1;
}

int isEmpty(const List *l) {
    if (l !=NULL) {
        if (l->sentinel_begin.next == &(l->sentinel_end)) {
            return 1;
        }
    }
    return 0;
}

int insertFirst(List *l, void *data, ListType dataType, int (*compareFunc)(void*, void*)) {
    if (l == NULL || data == NULL) return 0;  // Added check for NULL data
    int wasEmpty = isEmpty(l);
    NodeList *newNode = (NodeList *)malloc(sizeof(NodeList));
    if (!newNode) return 0;  // Allocation failed

    // Ensure the type matches
    if (l->type != dataType) {
        fprintf(stderr, "Error: Trying to insert the wrong type into the list.\n");
        free(newNode);  // Free the allocated memory for the node
        return 0;  // Return failure
    }

	//Checks for duplicates
	if(find_exact(l, data, compareFunc)) {
		fprintf(stderr, "Error: Duplicating element.\n");
		free(newNode);
		return 0;
	}

    newNode->data = data;
    newNode->next = l->sentinel_begin.next;
    newNode->previous = &(l->sentinel_begin);

    // Update the sentinel_begin's next pointer
    l->sentinel_begin.next = newNode;


    // If the list was empty, set sentinel_end.previous to the new node
    if (wasEmpty) {
        l->sentinel_end.previous = newNode;
        newNode->next = &(l->sentinel_end);  // Set the new node's next to sentinel_end
    }
    newNode->next->previous = newNode;
    return 1;  // Successful insertion
}

int isFirst(const List *l) {
    if (isEmpty(l) || l->current == NULL) return 0;
    return (l->current == l->sentinel_begin.next);
}

int isLast(const List *l) {
    if (l == NULL || l->current == NULL || isEmpty(l)) return 0;
    return (l->current == l->sentinel_end.previous);
}


int isOutOfList(const List *l) {
    if (l == NULL || l->current == NULL) {
        return 1; // NULL pointers are considered out of list
    }
    return 0;
}

void setOnFirst(List *l) {
    if (l==NULL || l->current ==NULL) return;
    if (isEmpty(l)) return;
    l->current = l->sentinel_begin.next;
}

void setOnLast(List *l) {
    if (l==NULL || l->current ==NULL) return;
    l->current = l->sentinel_end.previous;
}

void setOnNext(List *l) {
    if (l == NULL || l->current == NULL|| isEmpty(l)) return;

    if (l->current->next != &(l->sentinel_end)) {
        l->current = l->current->next;  // Move to the next node
    }
}

void setOnPrev(List *l) {
    if (l==NULL || l->current ==NULL) return;
    l->current = l->current->previous;
}

void * getCurrentData(const List * l) {
    if (l==NULL || l->current ==NULL) return NULL;
    if (isEmpty(l) || isOutOfList(l)) return NULL;
    return l->current->data;
}

int nbElement(List * l) {
    if (l==NULL || l->current ==NULL) return -1;
    if (!isEmpty(l)) {
        int count = 0;
        setOnFirst(l);
        while (!isLast(l)) {
            count ++;
            setOnNext(l);
        }
        return ++count;
    }
    fprintf(stderr, "List is empty.\n");
    return 0;
}

void printList(const List *l, void (*printFunc)(void*)) {
    if (l == NULL || isEmpty(l) || printFunc == NULL) {
        fprintf(stderr, "Invalid input or empty list.\n");
        return;
    }

    NodeList *current = l->sentinel_begin.next; // Start from the first real node

    while (current != &(l->sentinel_end)) {
        printFunc(current->data);  // Call the print function
        current = current->next;
    }
}

int find_exact(List *l, void *data, int (*compareFunc)(void*, void*)){
    if (l == NULL || data == NULL || compareFunc == NULL || isEmpty(l)) {
        return 0;
    }
    //Start the search from the first node
    NodeList *current = l->sentinel_begin.next;

    while (current != &(l->sentinel_end)) {
        if (compareFunc(current->data, data)) {
            l->current = current;
            return 1;  // Found
        }
        current = current->next;
    }

    return 0;  // Not found
}

int find_min(List *l, void *data, void *data_found, int (*compareFunc)(void*, void*)) {
    if (l == NULL || compareFunc == NULL || data ==NULL || data_found == NULL || isEmpty(l)) {
        return 0;
    }
    //Start the search from the first node
    NodeList *current = l->sentinel_begin.next;
    int mini = INT_MAX;
    void *minData = NULL;
    while (current != &(l->sentinel_end)) {
        int temp = compareFunc(current->data, data);
        if (temp<mini){
            mini=temp;
            minData = current->data;
        }
        current = current->next;
    }
    *(void **)data_found = minData;
    return mini;
}

int deleteValue(List *l, void *data, int (*compareFunc)(void*, void*), void (*deleteFunc)(void*)) {
    if (l == NULL || data == NULL || compareFunc == NULL) {
        return 0;
    }

    if (find_exact(l,data,compareFunc) == 1) {
        //find function move l->current to the Nodelist we want to delete
        NodeList *current = l->current;
        current->previous->next = current->next;
        current->next->previous = current->previous;

        //Update l->current
        l->current = current->next;
        //If next is the sentinel_end, update l->current to the previous one
        if (l->current == &(l->sentinel_end)) l->current = current->previous;

        //delete NodeList
        if (deleteFunc != NULL) {
            deleteFunc(current->data);
        }
        freeNodeList(current);
        return 1;
    }
    return 0;  // Not found
}

static void freeNodeList(NodeList * n) {
    free((void*)n);
}

//Use to free every node list of a list but wont free the list itself (avoid invalid read)
//For Color and Function, deleteFunc = NULL (only the structure itself is allocated)
//For Group, deleteFunc = deleteGroup
//For Light, deleteFunc = deleteLight
void deleteList(List *l, void (*deleteFunc)(void*)) {
    if (l == NULL) return;

    NodeList *current = l->sentinel_begin.next;
    while (current != &(l->sentinel_end)) {
        NodeList *toDelete = current;
        current = current->next;

        if (toDelete->data != NULL && deleteFunc != NULL) {
            deleteFunc(toDelete->data);
        }
        freeNodeList(toDelete);
    }
    free(l); // Always free the list structure itself
}

