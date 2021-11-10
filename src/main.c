#include <stdio.h>
#include "dynarr.h"

#define DBG(...) (fprintf(stderr, __VA_ARGS__), fflush(stderr))

int main()
{
	dynarr_t *arr = dynarr_init(sizeof(int));

	for (int i = 0; i < 100; ++i) {
		DBG("pushing %d\n", i);
		dynarr_push(arr, &i);
	}

	dynarr_push_rval(arr, 5);

	DBG("========\n");

	int tmp = *(int *) dynarr_pop_ptr(arr);

	DBG("count before pop: %zu\n", dynarr_count(arr));
	// int tmp;
	dynarr_pop(arr, &tmp);
	DBG("popped: %d\n", tmp);
	DBG("count after pop: %zu\n", dynarr_count(arr));

	tmp = 25;
	dynarr_insert(arr, 95, &tmp);

	dynarr_resize(arr, 40);

	for (size_t i = 0; i < dynarr_count(arr); ++i) {
		int tmp;
		dynarr_get(arr, i, &tmp);
		DBG("arr[%zu] = %d\n", i, tmp);
	}

	dynarr_free(arr);
	return 0;
}
