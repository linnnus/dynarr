#include "dynarr.h"

dynarr_t *dynarr_init_count(size_t element_size, size_t initial_cap)
{
	dynarr_t *ptr = malloc(sizeof(*ptr));
	assert(ptr != NULL);

	ptr->_count    = 0;
	ptr->_cap      = initial_cap;
	ptr->_elt_size = element_size;
	ptr->_data     = malloc(ptr->_cap * ptr->_elt_size);
	assert(ptr->_data != NULL);

	return ptr;
}

dynarr_t *dynarr_init(size_t element_size)
{
	return dynarr_init_count(element_size, DYNARR_DEFAULT_CAP);
}

void dynarr_free(dynarr_t *arr)
{
	if (arr == NULL) return;

	free(arr->_data);
	free(arr);
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

void dynarr_pop(dynarr_t *arr, void *dest)
{
	assert(arr  != NULL);
	assert(dest != NULL);
	assert(arr->_count > 0);

	--arr->_count;
	memcpy(dest, arr->_data + (arr->_count * arr->_elt_size),
	       arr->_elt_size);

	// TODO: resize downwards if (count == cap / 2) or smth
}

void dynarr_set(dynarr_t *arr, size_t idx, void *src)
{
	assert(arr != NULL);
	assert(idx < arr->_count);
	assert(src != NULL);

	memcpy(arr->_data + (idx * arr->_elt_size), src, arr->_elt_size);
}

void dynarr_get(dynarr_t *arr, size_t idx, void *dest)
{
	assert(arr != NULL);
	assert(idx < arr->_count);
	assert(dest != NULL);

	memcpy(dest, arr->_data + idx * arr->_elt_size, arr->_elt_size);
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

void dynarr_resize(dynarr_t *arr, size_t new_cap)
{
	assert(arr != NULL);

	arr->_cap = new_cap;
	if (new_cap < arr->_count)
		arr->_count = new_cap;

	arr->_data = realloc(arr->_data, arr->_cap * arr->_elt_size);
	assert(arr->_data != NULL);
}

size_t dynarr_count(dynarr_t *arr)
{
	assert(arr != NULL);

	return arr->_count;
}

// vim: ft=c noet tw=80
