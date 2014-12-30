#include <stdlib.h>
#include <stdio.h>
#include "../src/tab.h"
#include "minunit.h"

int tests_run = 0;

struct field_parse_triad
{
	const char*	value_string;
	double			value_float;
	long int		value_integer;
};

struct field_parse_triad field_tests[] = {
	{ .value_string = "2",				.value_float =  2.0,					.value_integer =  2					},
	{ .value_string = "-3",				.value_float = -3.0,					.value_integer = -3					},
	{ .value_string = "1e8",			.value_float =  100000000.0,	.value_integer =  100000000	},
	{ .value_string = "-1E2",			.value_float = -100.0,				.value_integer = -100				},
	{ .value_string = "15.3",			.value_float =  15.3,					.value_integer =  0					},
	{ .value_string = "-27.3447",	.value_float = -27.3447,			.value_integer =  0					},
	{ .value_string = "2e-1",			.value_float =  0.05,					.value_integer =  0					},
	{ .value_string = "27f34",		.value_float =  0.0,					.value_integer =  0					}
};
#define FIELD_TESTS_LEN	8

static char * test_field_parse (void)
{
	size_t				ix;
	/* char*					msg = malloc (255 * sizeof(char)); */
	tab_field_t*	f = tab_field_create (0);
	
	for (ix=0; ix<FIELD_TESTS_LEN; ix++)
	{
		tab_field_set_string_value (f, field_tests[ix].value_string);

		mu_assert(
			"error, tab_field_set_string_value did not correctly parse string into a floating point value",
			tab_field_get_float_value (f) == field_tests[ix].value_float
		);

		mu_assert(
			"error, tab_field_set_string_value did not correctly parse string into a integer value",
			tab_field_get_integer_value (f) == field_tests[ix].value_integer
		);
	}

	tab_field_destroy (f);
	return 0;
}

static char * all_field_tests (void)
{
	mu_run_test(test_field_parse);

	return 0;
}

static char * all_tests (void)
{
	mu_run_test(all_field_tests);

	return 0;
}

int main (int argc, char *argv[])
{
	char *result = all_tests();

	if (result != 0)
		printf("%s\n", result);
	else
		printf("ALL TESTS PASSED\n");

	printf("Tests Run: %d\n", tests_run);

	return result != 0;
}

