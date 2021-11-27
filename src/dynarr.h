#ifndef DYNARR_H
#define DYNARR_H

// TODO: messsages with assert macro?
// TODO: handle max size
// TODO: ensure that dynarr_{count,data} are inlined

#include <assert.h> // assert
#include <stddef.h> // size_t
#include <string.h> // memcpy

#if defined(DYNARR_REALLOC) != defined(DYNARR_FREE)
#error "You must either set both DYNARR_REALLOC and DYNARR_FREE but not both"
#endif

#ifndef DYNARR_REALLOC
#include <stdlib.h> // realloc, free
#define DYNARR_REALLOC realloc
#define DYNARR_FREE    free
#endif

#ifndef DYNARR_DEFAULT_CAP
#define DYNARR_DEFAULT_CAP 8
#endif

#ifndef DYNARR_RESIZE_FACTOR
#define DYNARR_RESIZE_FACTOR 2
#endif

// NOTE: the long names will still be available.
//       do we want this?

#ifdef DYNARR_SHORT_NAMES
#define arrpushr   dynarr_push_rval
#define arrsetr    dynarr_set_rval
#define arrinsertr dynarr_insert_rval

#define dynarr_init_count arrinitcnt
#define dynarr_init       arrinit
#define dynarr_resize     arrresize
#define dynarr_free       arrfree
#define dynarr_push       arrpush
#define dynarr_pop_ptr    arrpopp
#define dynarr_pop        arrpop
#define dynarr_set        arrset
#define dynarr_get_ptr    arrgetp
#define dynarr_get        arrget
#define dynarr_insert     arrinsert
#define dynarr_count      arrcount
#define dynarr_data       arrdata
#endif

// _rval variations of all functions that add new data to the array

#define dynarr_push_rval(arr, val) \
	do { \
		__auto_type tmp = val; \
		assert(sizeof(tmp) == arr->_elt_size); \
		dynarr_push(arr, &tmp); \
	} while (0)

#define dynarr_set_rval(arr, idx, val) \
	do { \
		__auto_type tmp = val; \
		assert(sizeof(tmp) == arr->_elt_size); \
		dynarr_set(arr, idx, &tmp); \
	} while (0)

#define dynarr_insert_rval(arr, idx, val) \
	do { \
		__auto_type tmp = val; \
		assert(sizeof(tmp) == arr->_elt_size); \
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
void dynarr_resize(dynarr_t *arr, size_t new_cap);
void dynarr_free(dynarr_t *arr);

void  dynarr_push(dynarr_t *arr, void *src);
void *dynarr_pop_ptr(dynarr_t *arr);
void  dynarr_pop(dynarr_t *arr, void *dest);
void  dynarr_set(dynarr_t *arr, size_t idx, void *src);
void *dynarr_get_ptr(dynarr_t *arr, size_t idx);
void  dynarr_get(dynarr_t *arr, size_t idx, void *dest);
void  dynarr_insert(dynarr_t *arr, size_t idx, void *src);

size_t dynarr_count(dynarr_t *arr);
void *dynarr_data(dynarr_t *arr);

#endif

#ifdef DYNARR_IMPLEMENTATION

dynarr_t *dynarr_init_count(size_t element_size, size_t initial_cap)
{
	dynarr_t *ptr = DYNARR_REALLOC(NULL, sizeof(*ptr));
	assert(ptr != NULL);

	ptr->_count    = 0;
	ptr->_cap      = initial_cap;
	ptr->_elt_size = element_size;
	ptr->_data     = DYNARR_REALLOC(NULL, ptr->_cap * ptr->_elt_size);
	assert(ptr->_data != NULL);

	return ptr;
}

dynarr_t *dynarr_init(size_t element_size)
{
	return dynarr_init_count(element_size, DYNARR_DEFAULT_CAP);
}

void dynarr_resize(dynarr_t *arr, size_t new_cap)
{
	assert(arr != NULL);

	arr->_cap = new_cap;
	if (new_cap < arr->_count)
		arr->_count = new_cap;

	arr->_data = DYNARR_REALLOC(arr->_data, arr->_cap * arr->_elt_size);
	assert(arr->_data != NULL);
}

void dynarr_free(dynarr_t *arr)
{
	if (arr == NULL) return;

	DYNARR_FREE(arr->_data);
	DYNARR_FREE(arr);
}

void dynarr_push(dynarr_t *arr, void *src)
{
	assert(arr != NULL);
	assert(src != NULL);

	// ensure enough space
	if (arr->_cap < (arr->_count + 1))
		dynarr_resize(arr, arr->_cap == 0
		              ? DYNARR_DEFAULT_CAP
		              : arr->_cap * DYNARR_RESIZE_FACTOR);

	// copy element
	memcpy(arr->_data + (arr->_count * arr->_elt_size),
	       src, arr->_elt_size);
	++arr->_count;

}

void *dynarr_pop_ptr(dynarr_t *arr)
{
	assert(arr  != NULL);
	assert(arr->_count > 0);

	--arr->_count;
	return arr->_data + (arr->_count * arr->_elt_size);
}

void dynarr_pop(dynarr_t *arr, void *dest)
{
	assert(arr  != NULL);
	assert(dest != NULL);
	assert(arr->_count > 0);

	memcpy(dest, dynarr_pop_ptr(arr), arr->_elt_size);
}

void dynarr_set(dynarr_t *arr, size_t idx, void *src)
{
	assert(arr != NULL);
	assert(idx < arr->_cap);
	assert(src != NULL);

	if (idx > arr->_count)
		arr->_count = idx + 1;

	memcpy(arr->_data + (idx * arr->_elt_size), src, arr->_elt_size);
}

void *dynarr_get_ptr(dynarr_t *arr, size_t idx)
{
	assert(arr != NULL);
	assert(idx < arr->_count);

	return arr->_data + idx * arr->_elt_size;
}

void dynarr_get(dynarr_t *arr, size_t idx, void *dest)
{
	assert(arr != NULL);
	assert(idx < arr->_count);
	assert(dest != NULL);

	memcpy(dest, dynarr_get_ptr(arr, idx), arr->_elt_size);
}

void dynarr_insert(dynarr_t *arr, size_t idx, void *src)
{
	assert(arr != NULL);
	assert(idx < arr->_count);
	assert(src != NULL);

	// ensure enough space
	if (arr->_cap < (arr->_count + 1))
		dynarr_resize(arr, arr->_cap == 0
		              ? DYNARR_DEFAULT_CAP
		              : arr->_cap * DYNARR_RESIZE_FACTOR);

	// cpy elements from idx to end one longer
	memcpy(arr->_data + (idx + 1) * arr->_elt_size,
	       arr->_data + idx * arr->_elt_size,
	       (arr->_count - idx) * arr->_elt_size);
	++arr->_count;

	// cpy src to idx (which is now undefined)
	memcpy(arr->_data + (idx * arr->_elt_size), src, arr->_elt_size);
}

size_t dynarr_count(dynarr_t *arr)
{
	assert(arr != NULL);

	return arr->_count;
}

void *dynarr_data(dynarr_t *arr)
{
	assert(arr != NULL);

	return arr->_data;
}

#endif

// vim: ft=c noet tw=80
