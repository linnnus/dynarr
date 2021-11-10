#ifndef DYNARR_H
#define DYNARR_H

// TODO: let user set malloc/realloc/free
// TODO: messsages with assert macro?
// TODO: handle max size

#include <assert.h> // assert
#include <stddef.h> // size_t
#include <stdlib.h> // malloc, realloc, free
#include <string.h> // memcpy

#ifndef DYNARR_DEFAULT_CAP
#define DYNARR_DEFAULT_CAP 8
#endif

#ifndef DYNARR_RESIZE_FACTOR
#define DYNARR_RESIZE_FACTOR 2
#endif

#define dynarr_push_rval(arr, val) \
	do { \
		__auto_type tmp = val; \
		dynarr_push(arr, &tmp); \
	} while (0)

#define dynarr_set_rval(arr, idx, val) \
	do { \
		__auto_type tmp = val; \
		dynarr_set(arr, idx, &tmp); \
	} while (0)

#define dynarr_insert_rval(arr, idx, val) \
	do { \
		__auto_type tmp = val; \
		dynarr_insert(arr, idx, &tmp); \
	} while (0)

typedef struct {
	void  *_data;
	size_t _cap;      // number of elements allocated
	size_t _count;    // number of elements in use
	size_t _elt_size; // size of each element
} dynarr_t;

dynarr_t *dynarr_init_count(size_t element_size, size_t initial_cap);
dynarr_t *dynarr_init(size_t element_size);
void dynarr_free(dynarr_t *arr);

void  dynarr_push(dynarr_t *arr, void *src);
void *dynarr_pop_ptr(dynarr_t *arr);
void  dynarr_pop(dynarr_t *arr, void *dest);
void  dynarr_set(dynarr_t *arr, size_t idx, void *src);
void *dynarr_get_ptr(dynarr_t *arr, size_t idx);
void  dynarr_get(dynarr_t *arr, size_t idx, void *dest);
void  dynarr_insert(dynarr_t *arr, size_t idx, void *src);

void dynarr_resize(dynarr_t *arr, size_t new_cap);
size_t dynarr_count(dynarr_t *arr);

#endif

// vim: ft=c noet tw=80
