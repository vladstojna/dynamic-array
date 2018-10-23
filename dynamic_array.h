/* Dynamic array header */

#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

/* Data type the array will hold */
typedef char da_data;

/* -------------------------------------------------------------------------- */

/* Dynamic array struct */
typedef struct {
	size_t capacity;
	size_t size;
	da_data *data;
} dynamic_array;

/* --------------------------------------------------------------------------
 * da_new()   - create dynamic array with capacity 1
 *              does not initialize array with a value
 *
 * da_new_n() - create dynamic array with capacity N
 *              does not initialize array with a value
 *
 * da_free()  - free resources used by the array
 *
 *
 * da_capacity() - array capacity; capacity >= 1
 *
 * da_size()     - number of elements in array; 0 <= size <= capacity
 *
 * da_first()    - first element; does not check size validity
 *
 * da_last()     - last element; does not check size validity
 *
 *
 * da_get() - get element from index; checks index validity
 *
 * da_set() - set value to index; checks index validity
 *
 *
 * da_push_back()  - appends an element to the end
 *
 * da_push_front() - appends an element to the start
 *
 * da_pop_back()   - removes and returns last element
 *                   does not check size validity
 *
 * da_pop_front()  - removes and returns first element
 *                   does not check size validity
 *
 *
 * da_insert()  - inserts an element to index; checks index validity
 *
 * da_append()  - append buffer to the end
 *
 *
 * da_rm_n() - remove n elements starting at index
 *             does not realloc array
 *             checks validity of parameters
 *
 * da_rm()   - remove element from index
 *             does not realloc array
 *             checks validity of parameters
 *
 * da_trim() - reallocs array so that capacity equals size
 *             realloc'd array capacity >= 1
 *
 * -------------------------------------------------------------------------- */

dynamic_array *da_new  ();
dynamic_array *da_new_n(size_t sz);
void           da_free (dynamic_array *d);

size_t  da_capacity(const dynamic_array *d);
size_t  da_size    (const dynamic_array *d);
da_data da_first   (const dynamic_array *d);
da_data da_last    (const dynamic_array *d);

da_data da_get(const dynamic_array *d, int i);
void    da_set(dynamic_array *d, int i, da_data val);

void    da_push_back (dynamic_array *d, da_data val);
void    da_push_front(dynamic_array *d, da_data val);
da_data da_pop_back  (dynamic_array *d);
da_data da_pop_front (dynamic_array *d);

void da_insert(dynamic_array *d, int i, da_data val);
void da_append(dynamic_array *dest, const da_data *src, size_t buffsz);

void da_rm_n(dynamic_array *d, int i, int n);
void da_rm  (dynamic_array *d, int i);

void da_trim(dynamic_array *d);


#endif
