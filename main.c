/* main */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynarr_t.h"
#include "dynamic_array.h"

int main() {

	dyn_array *d = dynarr_new();

	char buffer[] = "text_to_append_blablablasfasgfasgasgasgascasdas123";

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	int i = 'a';
	for (int j = 0; j < 17; j++, i++) {
		dynarr_push(d, i);
		printf("size = %3ld length = %3ld value = %3c\n",
			dynarr_size(d),
			dynarr_length(d),
			dynarr_get(d, j));
	}

	/* -------------------------------------------------------------------------- */

	printf(">> Appending buffer now: %s\n", buffer);

	dynarr_append(d, buffer, strlen(buffer));

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	for (int i = 0; i < d->length; i++) {
		printf("%c", dynarr_get(d, i));
	}
	printf("\n");

	/* -------------------------------------------------------------------------- */

	printf(">> Pushing another character\n");

	dynarr_push(d, 'f');

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	/* -------------------------------------------------------------------------- */

	printf(">> Trimming array\n");

	dynarr_trim(d);

	printf("[");
	for (int i = 0; i < d->length; i++) {
		printf(" %c", dynarr_get(d, i));
	}
	printf(" ]\n");

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	/* -------------------------------------------------------------------------- */

	printf(">> Starting removal now\n");

	dynarr_rmt_n(d, 0, 21);
	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	printf("[");
	for (int i = 0; i < d->length; i++) {
		printf(" %c", dynarr_get(d, i));
	}
	printf(" ]\n");

	/* -------------------------------------------------------------------------- */

	printf(">> Removing single character with trim\n");

	dynarr_rmt(d, 10);

	printf("[");
	for (int i = 0; i < d->length; i++) {
		printf(" %c", dynarr_get(d, i));
	}
	printf(" ]\n");

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	/* -------------------------------------------------------------------------- */

	printf(">> Popping from array\n");

	dynarr_pop(d);

	printf("[");
	for (int i = 0; i < d->length; i++) {
		printf(" %c", dynarr_get(d, i));
	}
	printf(" ]\n");

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	/* -------------------------------------------------------------------------- */

	printf(">> Dequeueing from array\n");

	dynarr_dequeue(d);

	printf("[");
	for (int i = 0; i < d->length; i++) {
		printf(" %c", dynarr_get(d, i));
	}
	printf(" ]\n");

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	/* -------------------------------------------------------------------------- */

	return EXIT_SUCCESS;
}
