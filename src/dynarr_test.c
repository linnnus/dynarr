#include "../include/acutest.h"

#define DYNARR_IMPLEMENTATION
#include "dynarr.h"

// TODO: test missing _rval variations
// TODO: test custom allocator
// TODO: test short names

// size to use in loops
#define TEST_COUNT 1000

struct test_struct {
	size_t a, b, c;
};

static void test_dynarr_init_count()
{
	dynarr_t *arr = dynarr_init_count(sizeof(int), 10);
	TEST_CHECK(arr->_count == 0);
	TEST_MSG("The count (amount in use) should be initialized to 0.");
	TEST_CHECK(arr->_cap == 10);
	TEST_MSG("The capacity (amount allocated) should be 10");
}

static void test_push_size_t()
{
	// use size_t for simplicity's sake to avoid casting
	dynarr_t *arr = dynarr_init(sizeof(size_t));

	for (size_t i = 0;  i < TEST_COUNT; ++i)
		dynarr_push(arr, &i);

	TEST_CHECK(dynarr_count(arr) ==  TEST_COUNT);

	for (size_t i = TEST_COUNT-1; i > 0; --i) {
		size_t tmp;
		dynarr_pop(arr, &tmp);

		TEST_CHECK(tmp == i);
		TEST_MSG("Expexted %zu but got %zu", i, tmp);
	}
}

static void test_push_size_t_ptr()
{
	dynarr_t *arr = dynarr_init(sizeof(size_t));

	for (size_t i = 0;  i < TEST_COUNT; ++i)
		dynarr_push(arr, &i);

	TEST_CHECK(dynarr_count(arr) == TEST_COUNT);

	for (size_t i = TEST_COUNT-1; i > 0; --i) {
		size_t tmp = * (size_t *) dynarr_pop_ptr(arr);

		TEST_CHECK(tmp == i);
		TEST_MSG("Expexted %zu but got %zu", i, tmp);
	}
}

static void test_push_struct()
{
	dynarr_t *arr = dynarr_init(sizeof(struct test_struct));

	for (size_t i = 0;  i < TEST_COUNT; ++i) {
		struct test_struct tmp = {
			.a = i,
			.b = i,
			.c = i,
		};
		dynarr_push(arr, &tmp);
	}

	TEST_CHECK(dynarr_count(arr) == TEST_COUNT);

	for (size_t i = TEST_COUNT-1; i > 0; --i) {
		struct test_struct tmp;
		dynarr_pop(arr, &tmp);

		TEST_CHECK(tmp.a = i && tmp.b == i && tmp.c == i);
		TEST_MSG("Expexted %zu but got %zu", i, tmp.a);
	}
}

static void test_push_struct_ptr()
{
	dynarr_t *arr = dynarr_init(sizeof(struct test_struct));

	for (size_t i = 0;  i < TEST_COUNT; ++i) {
		struct test_struct tmp = {
			.a = i,
			.b = i + 1,
			.c = i + 2,
		};
		dynarr_push(arr, &tmp);
	}

	TEST_CHECK(dynarr_count(arr) == TEST_COUNT);

	for (size_t i = TEST_COUNT-1; i > 0; --i) {
		struct test_struct *tmp = dynarr_pop_ptr(arr);

		TEST_CHECK(tmp->a = i && tmp->b == i + 1 && tmp->c == i + 2);
	}
}

static void test_set_size_t()
{
	dynarr_t *arr = dynarr_init_count(sizeof(size_t), TEST_COUNT);

	TEST_CHECK(dynarr_count(arr) == 0);
	TEST_MSG("Count should be initialized to 0");

	for (size_t i = 0; i < TEST_COUNT; ++i) {
		size_t tmp =  i;
		dynarr_set(arr, i, &tmp);
	}

	TEST_CHECK(dynarr_count(arr) == TEST_COUNT);
	TEST_MSG("Count should be increased to largest dynarr_set()");

	for (size_t i = 0; i < TEST_COUNT; ++i) {
		size_t tmp;
		dynarr_get(arr, i, &tmp);

		TEST_CHECK(tmp == i);
		TEST_MSG("Exptected %zu but got %zu", i, tmp);

		tmp = *(size_t *) dynarr_get_ptr(arr, i);

		TEST_CHECK(tmp == i);
		TEST_MSG("Exptected %zu but got %zu", i, tmp);
	}
}

static void test_set_struct()
{
	dynarr_t *arr = dynarr_init_count(sizeof(struct test_struct),
	                                  TEST_COUNT);

	TEST_CHECK(dynarr_count(arr) == 0);
	TEST_MSG("Count should be initialized to 0");

	for (size_t i = 0; i < TEST_COUNT; ++i) {
		struct test_struct tmp = {
			.a = i,
			.b = i + 1,
			.c = i + 2,
		};
		dynarr_set(arr, i, &tmp);
	}

	TEST_CHECK(dynarr_count(arr) == TEST_COUNT);
	TEST_MSG("Count should be increased to largest dynarr_set()");

	for (size_t i = 0; i < TEST_COUNT; ++i) {
		struct test_struct tmp;
		dynarr_get(arr, i, &tmp);

		TEST_CHECK(tmp.a == i && tmp.b == i + 1 && tmp.c == i + 2);

		struct test_struct *ptr = dynarr_get_ptr(arr, i);

		TEST_CHECK(ptr->a == i && ptr->b == i + 1 && ptr->c == i + 2);
	}
}

static void test_dynarr_insert()
{
	dynarr_t *arr = dynarr_init(sizeof(size_t));

	size_t tmp;
	tmp = 0;
	dynarr_push(arr, &tmp);
	tmp = 1;
	dynarr_push(arr, &tmp);
	tmp = 3;
	dynarr_push(arr, &tmp);

	tmp = 2;
	dynarr_insert(arr, 2, &tmp);

	for (size_t i = 0; i < 4; ++i) {
		size_t tmp;
		dynarr_get(arr, i, &tmp);
		TEST_CHECK(tmp == i);
		TEST_MSG("Expected %zu but got %zu", i, tmp);
	}
}

static void test_dynarr_resize()
{
	dynarr_t *arr = dynarr_init_count(sizeof(size_t), 10);

	dynarr_resize(arr, 20);
	TEST_CHECK(arr->_cap == 20);

	for (size_t i = 0; i < 10; ++i)
		dynarr_push(arr, &i);
	TEST_CHECK(dynarr_count(arr) == 10);

	dynarr_resize(arr, 5);
	TEST_CHECK(dynarr_count(arr) == 5);
}

static void test_dynarr_count()
{
	dynarr_t *arr = dynarr_init_count(sizeof(int), 10);

	TEST_CHECK(arr->_count == dynarr_count(arr));
}

static void test_dynarr_data()
{
	dynarr_t *arr = dynarr_init_count(sizeof(int), 10);

	TEST_CHECK(arr->_data == dynarr_data(arr));
}

static void test_dynarr_push_rval()
{
	dynarr_t *arr = dynarr_init(sizeof(size_t));

	// have to use UL so __auto_type doesn't assume int
	dynarr_push_rval(arr, 5UL);
	dynarr_push_rval(arr, 50UL);

	size_t tmp;
	dynarr_pop(arr, &tmp);
	TEST_CHECK(tmp == 50UL);
	TEST_MSG("Expected 50 but got %zu", tmp);

	dynarr_pop(arr, &tmp);
	TEST_CHECK(tmp == 5UL);
	TEST_MSG("Expected 5 but got %zu", tmp);
}

static void test_dynarr_set_rval()
{
	dynarr_t *arr = dynarr_init_count(sizeof(size_t), 10);

	dynarr_set_rval(arr, 7, 10UL);

	size_t *tmp = dynarr_get_ptr(arr, 7);
	TEST_CHECK(*tmp == 10UL);
}

static void test_dynarr_insert_rval()
{
	dynarr_t *arr = dynarr_init_count(sizeof(size_t), 3);

	dynarr_set_rval(arr, 0, 0UL);
	dynarr_set_rval(arr, 1, 2UL);

	dynarr_insert_rval(arr, 1, 1UL);

	for (size_t i = 0; i < dynarr_count(arr); ++i) {
		size_t tmp;
		dynarr_get(arr, i, &tmp);

		TEST_CHECK(tmp == i);
		TEST_MSG("Expected %zu at idx %zu but got %zu", i, i, tmp);
	}
}

TEST_LIST = {
	{ "dynarr_init_count",                 test_dynarr_init_count },
	{ "pushing/popping size_t",            test_push_size_t },
	{ "pushing/popping size_t using _ptr", test_push_size_t_ptr },
	{ "pushing/popping struct",            test_push_struct },
	{ "pushing/popping struct using _ptr", test_push_struct_ptr },
	{ "setting/getting size_t",            test_set_size_t },
	{ "setting/getting struct",            test_set_struct },
	{ "dynarr_insert",                     test_dynarr_insert },
	{ "dynarr_resize",                     test_dynarr_resize },
	{ "dynarr_count",                      test_dynarr_count },
	{ "dynarr_data",                       test_dynarr_data },
	{ "dynarr_push_rval",                  test_dynarr_push_rval },
	{ "dynarr_set_rval",                   test_dynarr_set_rval },
	{ "dynarr_insert_rval",                test_dynarr_insert_rval },
	{ NULL, NULL }
};

// vim: ft=c noet tw=80
