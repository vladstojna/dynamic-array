/* Dynamic array source */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

/* Array doubles when size exceeds capacity */
#define MULT 2

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

static int __da_reserve    (dynamic_array *d, size_t new_size);
static int __da_memcpy     (dynamic_array *d, int i, const da_data *src, size_t buffsz);
static int __da_shift_left (dynamic_array *d, int start, int amount);
static int __da_shift_right(dynamic_array *d, int start, int amount);

/* -------------------------------------------------------------------------- */

inline int __da_reserve(dynamic_array *d, size_t new_size) {
	da_data *data_new = (da_data*) realloc((da_data*) d->data, sizeof(da_data)*new_size);

	if (data_new == NULL) {
		/* free old */
		da_free(d);
		fprintf(stderr, "__da_reserve: realloc error\n");
		exit(EXIT_FAILURE);
	}

	d->data     = data_new;
	d->capacity = new_size;

	return EXIT_SUCCESS;
}

inline int __da_memcpy(dynamic_array *d, int i, const da_data *src, size_t buffsz) {
	if (memcpy(&d->data[i], src, sizeof(da_data) * buffsz) == NULL) {
		da_free(d);
		fprintf(stderr, "__da_memcpy: memcpy error\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

inline int __da_shift_left(dynamic_array *d, int start, int amount) {
	memmove(&d->data[start],
		&d->data[start+amount],
		sizeof(da_data) * (d->size - (start + amount)));

	return EXIT_SUCCESS;
}

inline int __da_shift_right(dynamic_array *d, int start, int amount) {
	memmove(&d->data[start+amount],
		&d->data[start],
		sizeof(da_data) * (d->size - start));

	return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */

dynamic_array *da_new_n(size_t sz) {
	dynamic_array *arr;
	/* alloc space */
	arr       = (dynamic_array*) malloc((size_t) sizeof(dynamic_array));
	arr->data = (da_data*)  malloc((size_t) sz * sizeof(da_data));
	/* init variables */
	arr->capacity = sz;
	arr->size     = 0;

	return arr;
}

dynamic_array *da_new() {
	return da_new_n(1);
}

void da_free(dynamic_array *a) {
	if (a != NULL) {
		free(a->data);
		free(a);
	}
}

/* -------------------------------------------------------------------------- */

size_t da_capacity(const dynamic_array *d) {
	return d->capacity;
}

size_t da_size(const dynamic_array *d) {
	return d->size;
}

da_data da_first(const dynamic_array *d) {
	return *(d->data);
}

da_data da_last(const dynamic_array *d) {
	return *(d->data + d->size - 1);
}

/* -------------------------------------------------------------------------- */

da_data da_get(const dynamic_array *d, int i) {
	if (i >= 0 && i < d->size)
		return d->data[i];
	fprintf(stderr, "da_get: index out of bounds %d\n", i);
	exit(EXIT_FAILURE);
}

void da_set(dynamic_array *d, int i, da_data val) {
	if (i < d->size && i > 0)
		d->data[i] = val;
	else {
		fprintf(stderr, "da_set: invalid index\n");
		exit(EXIT_FAILURE);
	}
}

/* -------------------------------------------------------------------------- */

void da_push_back(dynamic_array *d, da_data val) {
	/* If there's still space for insertion */
	if (d->size == d->capacity)
		__da_reserve(d, MULT * d->capacity);
	/* write value and update size */
	d->data[d->size] = val;
	d->size += 1;
}

void da_push_front(dynamic_array *d, da_data val) {
	/* If there's still space for insertion */
	if (d->size == d->capacity)
		__da_reserve(d, MULT * d->capacity);
	/* shift whole array right */
	__da_shift_right(d, 0, 1);
	/* write value and update size */
	d->data[0] = val;
	d->size += 1;
}

da_data da_pop_back(dynamic_array *d) {
	d->size -= 1;
	return d->data[d->size];
}

da_data da_pop_front(dynamic_array *d) {
	da_data front = *d->data;
	__da_shift_left(d, 0, 1);
	d->size -= 1;
	return front;
}

void da_insert(dynamic_array *d, int i, da_data val) {
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
		fprintf(stderr, "da_insert: invalid index\n");
		exit(EXIT_FAILURE);
	}
}

void da_append(dynamic_array *dest, const da_data *src, size_t buffsz) {
	/* check if there is enough space */
	if (dest->capacity - dest->size < buffsz)
		__da_reserve(dest, buffsz + dest->size);
	/* copy memory and update size */
	__da_memcpy(dest, dest->size, src, buffsz);
	dest->size += buffsz;
}

/* -------------------------------------------------------------------------- */

void da_rm_n(dynamic_array *d, int i, int n) {

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
		da_free(d);
		fprintf(stderr, "da_rm_n: invalid indices i=%d n=%d\n", i, n);
		exit(EXIT_FAILURE);
	}
}

void da_rm(dynamic_array *d, int i) {
	da_rm_n(d, i, 1);
}

void da_trim(dynamic_array *d) {
	/* if not full */
	if (d->size < d->capacity)
		__da_reserve(d, d->size);
}
