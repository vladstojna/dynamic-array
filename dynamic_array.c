/* Dynamic array source */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

/* Array doubles when size exceeds capacity */
#define MULT 2

/* Dynamic array struct */
struct __dynamic_array {
	size_t capacity;
	size_t size;
	da_data *data;
};

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
 *
 * __da_p2_ceil() - calculates lowest next power of 2 after n > 0
 * -------------------------------------------------------------------------- */

static int __da_reserve    (dynamic_array *d, size_t new_size);
static int __da_memcpy     (dynamic_array *d, int i, const da_data *src, size_t buffsz);
static int __da_shift_left (dynamic_array *d, int start, int amount);
static int __da_shift_right(dynamic_array *d, int start, int amount);
static int __da_p2_ceil    (int n);

/* -------------------------------------------------------------------------- */

int __da_reserve(dynamic_array *d, size_t new_size) {
	da_data *data_new = (da_data*) realloc((da_data*) d->data, sizeof(da_data)*new_size);

	if (data_new == NULL) {
		/* free old */
		da_free(d);
		fprintf(stderr, "__da_reserve: realloc error\n");
		return EXIT_FAILURE;
	}

	d->data     = data_new;
	d->capacity = new_size;

	return EXIT_SUCCESS;
}

int __da_memcpy(dynamic_array *d, int i, const da_data *src, size_t buffsz) {
	if (memcpy(&d->data[i], src, sizeof(da_data) * buffsz) == NULL) {
		da_free(d);
		fprintf(stderr, "__da_memcpy: memcpy error\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int __da_shift_left(dynamic_array *d, int start, int amount) {
	memmove(&d->data[start],
	        &d->data[start+amount],
	        sizeof(da_data) * (d->size - (start + amount)));

	return EXIT_SUCCESS;
}

int __da_shift_right(dynamic_array *d, int start, int amount) {
	memmove(&d->data[start+amount],
	        &d->data[start],
	        sizeof(da_data) * (d->size - start));

	return EXIT_SUCCESS;
}

int __da_p2_ceil(int n) {
	int p2 = 2;
	n--;
	while (n >>= 1)
		p2 <<= 1;
	return p2;
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
	return da_new_n(16);
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

da_data *da_get_data(const dynamic_array *d) {
	return d->data;
}

da_data da_first(const dynamic_array *d) {
	return *(d->data);
}

da_data da_last(const dynamic_array *d) {
	return *(d->data + d->size - 1);
}

/* -------------------------------------------------------------------------- */

da_data da_get(const dynamic_array *d, int i) {
	if (i < 0 || i >= d->size) {
		fprintf(stderr, "da_get: index out of bounds %d\n", i);
		return -1;
	}
	return d->data[i];
}

int da_set(dynamic_array *d, int i, da_data val) {
	if (i < 0 || i >= d->size) {
		fprintf(stderr, "da_set: index out of bounds %d\n", i);
		return EXIT_FAILURE;
	}
	d->data[i] = val;
	return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */

int da_push_back(dynamic_array *d, da_data val) {
	/* If there's still space for insertion */
	if (d->size == d->capacity)
		if (__da_reserve(d, MULT * d->capacity))
			return EXIT_FAILURE;
	/* write value and update size */
	d->data[d->size] = val;
	d->size += 1;
	return EXIT_SUCCESS;
}

int da_push_front(dynamic_array *d, da_data val) {
	/* If there's still space for insertion */
	if (d->size == d->capacity)
		if (__da_reserve(d, MULT * d->capacity))
			return EXIT_FAILURE;
	/* shift whole array right */
	__da_shift_right(d, 0, 1);
	/* write value and update size */
	d->data[0] = val;
	d->size += 1;
	return EXIT_SUCCESS;
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

int da_insert(dynamic_array *d, int i, da_data val) {
	if (i >= 0 && i <= d->size) {
		/* check available space to insert */
		if (d->size == d->capacity)
			if (__da_reserve(d, MULT * d->capacity))
				return EXIT_FAILURE;
		/* shift when inserting in the middle */
		if (i < d->size)
			__da_shift_right(d, i, 1);
		/* write value and update size */
		d->data[i] = val;
		d->size += 1;
		return EXIT_SUCCESS;
	}
	else {
		fprintf(stderr, "da_insert: invalid index\n");
		return EXIT_FAILURE;
	}
}

int da_append(dynamic_array *dest, const da_data *src, size_t buffsz) {
	/* check if there is enough space */
	if (dest->capacity - dest->size < buffsz)
		if (__da_reserve(dest, __da_p2_ceil((buffsz + dest->size) / dest->capacity) * dest->capacity))
			return EXIT_FAILURE;
	/* copy memory and update size */
	if (__da_memcpy(dest, dest->size, src, buffsz))
		return EXIT_FAILURE;
	dest->size += buffsz;
	return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */

int da_rm_n(dynamic_array *d, int i, int n) {

	/* no removal needed */
	if (n == 0) return EXIT_SUCCESS;

	/* args validity check */
	if (i >= 0 && n > 0 && i < d->size) {
		/* only shift if n is less than remaining size */
		if (i + n < d->size) {
			__da_shift_left(d, i, n);
			d->size -= n;
		}
		else
			d->size = i;
		return EXIT_SUCCESS;
	}
	else {
		da_free(d);
		fprintf(stderr, "da_rm_n: invalid indices i=%d n=%d\n", i, n);
		return EXIT_FAILURE;
	}
}

int da_rm(dynamic_array *d, int i) {
	return da_rm_n(d, i, 1);
}

int da_trim(dynamic_array *d) {
	/* if not full */
	if (d->size < d->capacity) {
		return __da_reserve(d, d->size == 0 ? 1 : d->size);
	}
	return EXIT_SUCCESS;
}

void da_clear(dynamic_array *d) {
	d->size = 0;
}
