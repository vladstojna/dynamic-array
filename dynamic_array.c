/* Dynamic array source */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynarr_t.h"
#include "dynamic_array.h"

/* Array doubles when size exceeds capacity */
#define MULT 2
#define MAX_SIZE 1073741823

/* Static functions */
/* --------------------------------------------------------------------------
 * __da_reserve() - reservers a new size for the array
 *                  does not check size
 *                  updates capacity and data pointer
 * 
 * __da_memcpy() - copies buffer to the array starting at index i
 *                 does not check index and array size
 * 
 * __da_shift_left() - shifts array left amount times starting at start
 *                     does not check indices
 * 
 * __da_shift_right() - shifts array right amount times starting at start
 *                      does not check indices
 * -------------------------------------------------------------------------- */

static int __da_reserve    (dyn_array *d, size_t new_size);
static int __da_memcpy     (dyn_array *d, int i, const dynarr_t *src, size_t buffsz);
static int __da_shift_left (dyn_array *d, int start, int amount);
static int __da_shift_right(dyn_array *d, int start, int amount);

/* -------------------------------------------------------------------------- */

inline int __da_reserve(dyn_array *d, size_t new_size) {
	dynarr_t *data_new = (dynarr_t*) realloc((dynarr_t*) d->data, sizeof(dynarr_t)*new_size);

	if (data_new == NULL) {
		/* free old */
		dynarr_free(d);
		fprintf(stderr, "__da_reserve: realloc error\n");
		exit(EXIT_FAILURE);
	}

	d->data     = data_new;
	d->capacity = new_size;

	return EXIT_SUCCESS;
}

inline int __da_memcpy(dyn_array *d, int i, const dynarr_t *src, size_t buffsz) {
	if (memcpy(&d->data[i], src, sizeof(dynarr_t) * buffsz) == NULL) {
		dynarr_free(d);
		fprintf(stderr, "__da_memcpy: memcpy error\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

inline int __da_shift_left(dyn_array *d, int start, int amount) {
	memmove(&d->data[start],
		&d->data[start+amount],
		sizeof(dynarr_t) * (d->size - (start + amount)));

	return EXIT_SUCCESS;
}

inline int __da_shift_right(dyn_array *d, int start, int amount) {
	memmove(&d->data[start+amount],
		&d->data[start],
		sizeof(dynarr_t) * (d->size - start));

	return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */

dyn_array *dynarr_new_n(size_t sz) {
	dyn_array *arr;
	/* alloc space */
	arr       = (dyn_array*) malloc((size_t) sizeof(dyn_array));
	arr->data = (dynarr_t*)  malloc((size_t) sz * sizeof(dynarr_t));
	/* init variables */
	arr->capacity = sz;
	arr->size     = 0;
	arr->first    = NULL;
	arr->last     = NULL;

	return arr;
}

dyn_array *dynarr_new() {
	return dynarr_new_n(1);
}

void dynarr_free(dyn_array *a) {
	if (a != NULL) {
		free(a->data);
		free(a);
	}
}

/* -------------------------------------------------------------------------- */

size_t dynarr_capacity(const dyn_array *d) {
	return d->capacity;
}

size_t dynarr_size(const dyn_array *d) {
	return d->size;
}

dynarr_t dynarr_first(const dyn_array *d) {
	return *(d->first);
}

dynarr_t dynarr_last(const dyn_array *d) {
	return *(d->last);
}

/* -------------------------------------------------------------------------- */

dynarr_t dynarr_get(const dyn_array *d, int i) {
	if (i >= 0 && i < d->size)
		return d->data[i];
	fprintf(stderr, "dynarr_get: index out of bounds %d\n", i);
	exit(EXIT_FAILURE);
}

void dynarr_set(dyn_array *d, int i, dynarr_t val) {
	if (i < d->size && i > 0)
		d->data[i] = val;
	else {
		fprintf(stderr, "dynarr_set: invalid index\n");
		exit(EXIT_FAILURE);
	}
}

/* -------------------------------------------------------------------------- */

void dynarr_push(dyn_array *dest, dynarr_t val) {
	/* If there's still space for insertion */
	if (dest->size == dest->capacity)
		__da_reserve(dest, MULT * dest->capacity);
	/* write value and update size */
	dest->data[dest->size] = val;
	dest->size += 1;
}

void dynarr_insert(dyn_array *d, int i, dynarr_t val) {
	if (i >= 0 && i < d->size) {
		/* If there's still space for insertion */
		if (d->size == d->capacity)
			__da_reserve(d, MULT * d->capacity);
		/* If inserting in the middle then shift */
		if (i < d->size)
			__da_shift_right(d, i, 1);
		/* write value and update size */
		d->data[i] = val;
		d->size += 1;
	}
	else {
		fprintf(stderr, "dynarr_insert: invalid index\n");
		exit(EXIT_FAILURE);
	}
}

void dynarr_enqueue(dyn_array *dest, dynarr_t val) {
	dynarr_push(dest, val);
}

void dynarr_append(dyn_array *dest, const dynarr_t *src, size_t buffsz) {
	/* check if there is enough space */
	if (dest->capacity - dest->size < buffsz)
		__da_reserve(dest, buffsz + dest->size);
	/* copy memory and update size */
	__da_memcpy(dest, dest->size, src, buffsz);
	dest->size += buffsz;
}

void dynarr_trim(dyn_array *d) {
	/* if not full */
	if (d->size < d->capacity)
		__da_reserve(d, d->size);
}

/* -------------------------------------------------------------------------- */

void dynarr_pop(dyn_array *d) {
	dynarr_rm(d, d->size - 1);
}

void dynarr_dequeue(dyn_array *d) {
	dynarr_rm(d, 0);
}

void dynarr_rm_n(dyn_array *d, int i, int n) {

	/* no removal needed */
	if (n == 0) return;

	/* args validity check */
	if (i >= 0 && n > 0 && i < d->size) {
		/* only shift if n is less than remaining size */
		if (i + n < d->size) {
			__da_shift_left(d, i, n);
			d->size -= n;
		}
		else
			d->size = i;
	}

	else {
		dynarr_free(d);
		fprintf(stderr, "dynarr_rm_n: invalid indices i=%d n=%d\n", i, n);
		exit(EXIT_FAILURE);
	}
}

void dynarr_rm(dyn_array *d, int i) {
	dynarr_rm_n(d, i, 1);
}
