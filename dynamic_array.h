/* Dynamic array header */

#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include "dynarr_t.h"

/* -------------------------------------------------------------------------- */

// TODO: first and last pointers / indices

/* Struct definition */
typedef struct dynamic_array_t {
	/* size   - total size
	   length - used size
	   first  - first useful 
	   last   - last useful  */
	size_t size;
	size_t length;
	dynarr_t *first;
	dynarr_t *last;
	/* data array */
	dynarr_t *data;
} dyn_array;

/* -------------------------------------------------------------------------- */

/* Creates a new dynamic array with size 1 */
dyn_array *dynarr_new();

/* Creates a new dynamic array with size sz */
dyn_array *dynarr_new_n(size_t sz);

/* Frees dynamic array */
void dynarr_free(dyn_array *d);

/* -------------------------------------------------------------------------- */

/* Returns array size (used and unused length) */
size_t dynarr_size(const dyn_array *d);

/* Returns array used length */
size_t dynarr_length(const dyn_array *d);

/* Returns first useful element from array */
dynarr_t dynarr_first(const dyn_array *d);

/* Returns last useful element from array */
dynarr_t dynarr_last(const dyn_array *d);

/* -------------------------------------------------------------------------- */

/* Returns element with index i from array */
dynarr_t dynarr_get(const dyn_array *d, int i);

/* Sets a value to valid index */
void dynarr_set(dyn_array *d, int i, dynarr_t val);

/* -------------------------------------------------------------------------- */

/* Pushes a value to the end of the array */
void dynarr_push(dyn_array *dest, dynarr_t val);

/* Enqueues a value to the array
   Same as dynarr_push */
void dynarr_enqueue(dyn_array *dest, dynarr_t val);

/* Appends buffer to array */
void dynarr_append(dyn_array *dest, const dynarr_t *src, size_t buffsz);

/* Trims array to useful size */
void dynarr_trim(dyn_array *dest);

/* -------------------------------------------------------------------------- */

/* Pops from array */
void dynarr_pop(dyn_array *d);

/* De-queue from array */
void dynarr_dequeue(dyn_array *d);

/* Removes n elements from index i
   Auto trims array when new length << size */
void dynarr_rm_n(dyn_array *d, int i, int n);

/* Removes element from index
   Auto trims array when new length << size */
void dynarr_rm(dyn_array *d, int i);

/* Removes n elements from index i
   Always trims result */
void dynarr_rmt_n(dyn_array *d, int f, int l);

/* Removes element from index and trims result */
void dynarr_rmt(dyn_array *d, int i);


#endif
