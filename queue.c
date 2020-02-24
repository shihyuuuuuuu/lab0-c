#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    // Return NULL if malloc failed
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    /* Free the list elements and the strings */
    list_ele_t *ptr = q->head;
    list_ele_t *tmp;
    while (ptr) {
        tmp = ptr->next;
        free(ptr->value);
        free(ptr);
        ptr = tmp;
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    size_t bufsize = (strlen(s) + 1) * sizeof(char);

    if (!q)
        return false;
    /* If either call to malloc returns NULL, return false. */
    if (!(newh = malloc(sizeof(list_ele_t))))
        return false;
    if (!(newh->value = malloc(bufsize))) {
        free(newh);
        return false;
    }
    strlcpy(newh->value, s, bufsize);
    newh->next = q->head;
    q->head = newh;
    q->size++;

    // If newh is the only node in the queue, point q->tail to it.
    if (!newh->next)
        q->tail = newh;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    list_ele_t *newt;
    size_t bufsize = (strlen(s) + 1) * sizeof(char);
    if (!q)
        return false;
    /* If either call to malloc returns NULL, return false. */
    if (!(newt = malloc(sizeof(list_ele_t))))
        return false;
    if (!(newt->value = malloc(bufsize))) {
        free(newt);
        return false;
    }
    strlcpy(newt->value, s, bufsize);
    newt->next = NULL;

    // TODO: Think of a way to avoid if-else statement.
    // If the size of queue is zero, insert element at head of queue.
    if (q_size(q) == 0)
        q->head = newt;
    // Else, insert element at tail of queue.
    // Add 'tail' field to the queue structure to ensure O(1) operation time.
    else
        q->tail->next = newt;
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q_size(q) == 0)
        return false;

    list_ele_t *to_del = q->head;
    if (sp) {
        memset(sp, '\0', bufsize);
        // Original code: strncpy(sp, to_del->value, bufsize);
        // TODO: Think about why the above code doesn't work
        strlcpy(sp, to_del->value, bufsize);
    }
    q->head = q->head->next;
    q->size--;
    free(to_del->value);
    free(to_del);
    if (!q->head)
        q->tail = NULL;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    // Add 'size' field to the queue structure to ensure O(1) operation time.
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q_size(q) == 0 || q_size(q) == 1)
        return;

    list_ele_t *prev, *node, *next;

    prev = q->head;
    node = prev->next;
    q->tail = prev;
    prev->next = NULL;
    /* Iterate the list. If the last node is reached, let q->head point to it.
     */
    while (node) {
        next = node->next;
        node->next = prev;
        prev = node;
        node = next;
    }
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    // TODO: Change the below bubble sort to merge sort
    if (!q || q_size(q) == 0)
        return;

    list_ele_t *tmp;
    list_ele_t *curr;
    list_ele_t *prev;
    list_ele_t *tail = NULL;

    while (q->head != tail) {
        curr = q->head;
        prev = q->head;
        while (curr && curr->next && curr->next != tail) {
            if ((curr->value)[0] > (curr->next->value)[0]) {
                tmp = curr->next;
                curr->next = tmp->next;
                tmp->next = curr;
                if (curr == q->head) {
                    prev = tmp;
                    q->head = tmp;
                } else {
                    prev->next = tmp;
                    prev = prev->next;
                }
            } else {
                if (curr != q->head) {
                    prev = prev->next;
                }
                curr = curr->next;
            }
        }
        tail = curr;
    }
    q->tail = tail;
}
