#include "tabular.h"
#include <stdlib.h>

struct	tab_column;
struct	tab_field;

union tab_data
{
	long int*		_integer;
	double*			_float;
	char*				_string;
};

struct tab_table
{
	size_t							_row_count;
	size_t							_col_count;
	size_t							_col_alloc;
	struct tab_column*	_cols;
};

struct tab_column
{
	enum tab_type				_type;
	size_t							_row_alloc;
	struct tab_field*		_rows;
};

struct tab_field
{
	enum tab_type				_type;
	union tab_data			_data;
};

