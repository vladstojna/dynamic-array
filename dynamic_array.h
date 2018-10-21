/* Dynamic array header */

#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include "dynarr_t.h"

/* -------------------------------------------------------------------------- */

/* Dynamic array struct */
typedef struct dynamic_array {
	size_t capacity;
	size_t size;
	size_t max_size;
	dynarr_t *first;
	dynarr_t *last;
	dynarr_t *data;
} dyn_array;

/* --------------------------------------------------------------------------
 * dynarr_new()   - create dynamic array with size 1
 *
 * dynarr_new_n() - create dynamic array with size N
 *
 * dynarr_free()  - free resources used by the array
 *
 *
 * dynarr_capacity() - array capacity
 *
 * dynarr_size()     - number of elements in array
 *
 * dynarr_first()    - pointer to first element
 *
 * dynarr_last()     - pointer to last element
 *
 *
 * dynarr_get() - get element from index
 *
 * dynarr_set() - set value to index
 *
 *
 * dynarr_push()    - appends an element to the end
 *
 * dynarr_insert()  - inserts an element to index
 *
 * dynarr_enqueue() - same as dynarr_push
 *
 * dynarr_append()  - append buffer to the end
 *
 * dynarr_trim()    - resizes array to fit number of elements
 *
 *
 * dynarr_pop()     - remove last element
 *
 * dynarr_dequeue() - remove first element
 *
 *
 * dynarr_rm_n()  - remove n elements starting at index
 *                  trims array when size is half capacity
 *
 * dynarr_rm()    - remove element from index
 *                  trims array when size is half capacity
 *
 * dynarr_rmt_n() - remove n elements starting at index
 *                  always trims array after removal
 *
 * dynarr_rmt()   - remove n elements starting at index
 *                  always trims array after removal
 * -------------------------------------------------------------------------- */

dyn_array *dynarr_new  ();
dyn_array *dynarr_new_n(size_t sz);
void       dynarr_free (dyn_array *d);

size_t   dynarr_capacity(const dyn_array *d);
size_t   dynarr_size    (const dyn_array *d);
dynarr_t dynarr_first   (const dyn_array *d);
dynarr_t dynarr_last    (const dyn_array *d);

dynarr_t dynarr_get(const dyn_array *d, int i);
void     dynarr_set(dyn_array *d, int i, dynarr_t val);

void dynarr_push   (dyn_array *dest, dynarr_t val);
void dynarr_insert (dyn_array *dest, int i, dynarr_t val);
void dynarr_enqueue(dyn_array *dest, dynarr_t val);
void dynarr_append (dyn_array *dest, const dynarr_t *src, size_t buffsz);
void dynarr_trim   (dyn_array *dest);

void dynarr_pop    (dyn_array *d);
void dynarr_dequeue(dyn_array *d);

void dynarr_rm_n (dyn_array *d, int i, int n);
void dynarr_rm   (dyn_array *d, int i);


#endif
