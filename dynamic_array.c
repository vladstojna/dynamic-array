  /* Dynamic array source */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynarr_t.h"
#include "dynamic_array.h"

/* Value by which to multiply array size when full */
#define MULT 2

/* -------------------------------------------------------------------------- */

/* Creates a new dynamic array with size sz */
dyn_array *dynarr_new_n(size_t sz) {
	dyn_array *arr;
	/* alloc space */
	arr       = (dyn_array*) malloc((size_t) sizeof(dyn_array));
	arr->data = (dynarr_t*)  malloc((size_t) sz * sizeof(dynarr_t));
	/* init variables */
	arr->size   = sz;
	arr->length = 0;
	arr->first  = NULL;
	arr->last   = NULL;

	return arr;
}

/* Creates a new dynamic array with size 1 */
dyn_array *dynarr_new() {
	return dynarr_new_n(1);
}

/* Frees dynamic array */
void dynarr_free(dyn_array *a) {
	if (a != NULL) {
		free(a->data);
		free(a);
	}
}

/* -------------------------------------------------------------------------- */

/* Returns array size (used and unused length) */
size_t dynarr_size(const dyn_array *d) {
	return d->size;
}

/* Returns array used length */
size_t dynarr_length(const dyn_array *d) {
	return d->length;
}

/* Returns first useful element from array */
dynarr_t dynarr_first(const dyn_array *d) {
	return *(d->first);
}

/* Returns last useful element from array */
dynarr_t dynarr_last(const dyn_array *d) {
	return *(d->last);
}

/* -------------------------------------------------------------------------- */

/* Returns element with index i from array */
dynarr_t dynarr_get(const dyn_array *d, int i) {
	if (i >= 0 && i < d->length)
		return d->data[i];
	fprintf(stderr, "dynarr_get: index out of bounds %d\n", i);
	exit(EXIT_FAILURE);
}

/* Sets a value to valid index */
void dynarr_set(dyn_array *d, int i, dynarr_t val) {
	if (i < d->length && i > 0)
		d->data[i] = val;
	else {
		fprintf(stderr, "dynarr_set: invalid index\n");
		exit(EXIT_FAILURE);
	}
}

/* -------------------------------------------------------------------------- */

/* Pushes a value to the end of the array */
void dynarr_push(dyn_array *dest, dynarr_t val) {
	/* If there's still space for insertion */
	if (dest->length < dest->size)
		dest->data[dest->length] = val;
	/* Else increase array size */
	else {
		size_t    new_sz = sizeof(dynarr_t) * MULT * dest->size;
		dynarr_t *data_new;
		/* realloc */
		data_new = (dynarr_t*) realloc((dynarr_t*) dest->data, new_sz);

		if (data_new == NULL) {
			/* free old */
			dynarr_free(dest);
			fprintf(stderr, "dynarr_append: realloc error\n");
			exit(EXIT_FAILURE);
		}

		/* update vars */
		dest->data = data_new;
		dest->size *= MULT;
		dest->data[dest->length] = val;
	}
	/* update length */
	dest->length += 1;
}

/* Enqueues a value to the array
   Same as dynarr_push */
void dynarr_enqueue(dyn_array *dest, dynarr_t val) {
	dynarr_push(dest, val);
}

/* Appends buffer to array */
void dynarr_append(dyn_array *dest, const dynarr_t *src, size_t buffsz) {
	/* check if there is enough space */
	if (dest->size - dest->length >= buffsz) {
		if (memcpy(&dest->data[dest->length], src, sizeof(dynarr_t) * buffsz) == NULL) {
			dynarr_free(dest);
			fprintf(stderr, "dynarr_append: memcpy error\n");
			exit(EXIT_FAILURE);
		}
	}
	/* else allocate more memory then copy */
	else {
		size_t    new_sz = sizeof(dynarr_t) * (buffsz + dest->length);
		dynarr_t *data_new;

		/* realloc */
		data_new = (dynarr_t*) realloc((dynarr_t*) dest->data, new_sz);

		if (data_new == NULL) {
			/* free old */
			dynarr_free(dest);
			fprintf(stderr, "dynarr_append: realloc error\n");
			exit(EXIT_FAILURE);
		}

		/* update vars */
		dest->data = data_new;
		dest->size = new_sz;

		/* copy memory */
		if (memcpy(&dest->data[dest->length], src, sizeof(dynarr_t) * buffsz) == NULL) {
			dynarr_free(dest);
			fprintf(stderr, "dynarr_append: memcpy error\n");
			exit(EXIT_FAILURE);
		}
	}
	/* update length */
	dest->length += buffsz;
}

/* Trims array to useful size */
void dynarr_trim(dyn_array *d) {
	/* if not full */
	if (d->length < d->size) {
		size_t    new_sz = sizeof(dynarr_t) * d->length;
		dynarr_t *data_new;

		/* realloc */
		data_new = (dynarr_t*) realloc((dynarr_t*) d->data, new_sz);

		if (d->data == NULL) {
			dynarr_free(d);
			fprintf(stderr, "dynarr_append: realloc error\n");
			exit(EXIT_FAILURE);
		}

		/* update vars */
		d->data = data_new;
		d->size = d->length;
	}
}

/* -------------------------------------------------------------------------- */

/* Pops from array */
void dynarr_pop(dyn_array *d) {
	dynarr_rmt(d, d->length - 1);
}

/* De-queue from array */
void dynarr_dequeue(dyn_array *d) {
	dynarr_rmt(d, 0);
}

/* Removes n elements from index i
   Auto trims array when new length << size */
void dynarr_rm_n(dyn_array *d, int i, int n) {

	/* no removal needed */
	if (n == 0) return;

	/* args validity check */
	if (i >= 0 && n > 0 && i < d->length) {

		/* shift only if n is small enough
		   shifts data left after i */
		for ( ; i + n < d->length; i++)
			d->data[i] = d->data[i+n];

		/* updates length */
		d->length -= n;
		/* trims when length is halved */
		if (d->size >= MULT * d->length)
			dynarr_trim(d);
	}

	else {
		dynarr_free(d);
		fprintf(stderr, "dynarr_rm_n: invalid indices i=%d n=%d\n", i, n);
		exit(EXIT_FAILURE);
	}
}

/* Removes element from index
   Auto trims array when new length << size */
void dynarr_rm(dyn_array *d, int i) {
	dynarr_rm_n(d, i, 1);
}

/* Removes element from index and trims result */
void dynarr_rmt_n(dyn_array *d, int i, int n) {

	/* no removal needed */
	if (n == 0) return;

	/* args validity check */
	if (i >= 0 && n > 0 && i < d->length) {

		/* shift only if n is small enough
		   shifts data left after i */
		for ( ; i + n < d->length; i++)
			d->data[i] = d->data[i+n];

		/* updates length */
		d->length -= n;
		/* trims */
		dynarr_trim(d);
	}

	else {
		dynarr_free(d);
		fprintf(stderr, "dynarr_rm_n: invalid indices i=%d n=%d\n", i, n);
		exit(EXIT_FAILURE);
	}
}

/* Removes element from index and trims result */
void dynarr_rmt(dyn_array *d, int i) {
	dynarr_rmt_n(d, i, 1);
}
