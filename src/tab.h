#ifndef TAB_H
#define TAB_H

#include <stdbool.h>

#define	TAB_DEFAULT_VALUE_INTEGER	0
#define	TAB_DEFAULT_VALUE_FLOAT		0.0
#define	TAB_DEFAULT_VALUE_STRING	"\0"

/****************************************************************************
 * General Tabular Declarations, Definitions, and Functions
 ****************************************************************************/
enum tab_datatype_e
{
	TAB_INTEGER,
	TAB_FLOAT,
	TAB_STRING
};
typedef enum tab_datatype_e		tab_dt_t;

/****************************************************************************
 * struct tab_field_s Declarations, Typedefs and Related Functions
 ****************************************************************************/
struct tab_field_s
{
	tab_dt_t		_datatype;
	long int	 	_value_integer;
	double		 	_value_float;
	char			*	_value_string;
};
typedef struct tab_field_s		tab_field_t;

/****************************************************************************
 * Use _initialize and _terminate if you want the functions to allocate and
 * deallocate memory, use _create and _destroy if the object already exists
 * _create calls _initialize internally, and _destroy calls _terminate
 */
bool						tab_field_initialize					(tab_field_t*, const char*);
bool						tab_field_terminate						(tab_field_t*);
tab_field_t*		tab_field_create							(const char*);
void						tab_field_destroy							(tab_field_t*);

/*****************************************************************************
 * Setters and Getters
 */
tab_dt_t				tab_field_get_datatype				(const tab_field_t*);
bool						tab_field_set_datatype				(tab_field_t*, tab_dt_t);

long int				tab_field_get_integer_value		(const tab_field_t*);
bool						tab_field_set_integer_value		(tab_field_t*, const long int);

double					tab_field_get_float_value			(const tab_field_t*);
bool						tab_field_set_float_value			(tab_field_t*, const double);

const char *		tab_field_get_string_value		(const tab_field_t*);
bool						tab_field_set_string_value		(tab_field_t*, const char *);

/*****************************************************************************
 * Utility Functions
 */
void						tab_field_print_details				(const tab_field_t*);


struct tab_column_s;
struct tab_table_s;

typedef struct tab_column_s		tab_column_t;
typedef struct tab_table_s		tab_table_t;

#endif /* TAB_H */
