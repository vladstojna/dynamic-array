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

	printf(">> Appending buffer now: %s\n", buffer);

	dynarr_append(d, buffer, strlen(buffer));

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	for (int i = 0; i < d->length; i++) {
		printf("%c", dynarr_get(d, i));
	}
	printf("\n");

	printf(">> Pushin another character\n");

	dynarr_push(d, 'f');

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	printf(">> Trimming array\n");

	dynarr_trim(d);

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	printf(">> Starting removal now\n");

	for (int i = 0; i < 34; i ++) {
		dynarr_rm(d, i);
		printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));
	}

	printf(">> Removing single character with trim\n");

	dynarr_rmt(d, 0);

	printf("size = %3ld length = %3ld\n", dynarr_size(d), dynarr_length(d));

	return EXIT_SUCCESS;
}
