#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "tab.h"

#define DEBUG 0

tab_field_t* _tab_parse_value (const char*);

bool
tab_field_initialize (tab_field_t* f, const char* val)
{
	tab_field_t	*p;

	// just in case the struct has allready been initialized
	tab_field_terminate (f);

	f->_datatype			= TAB_STRING;
	f->_value_integer	= TAB_DEFAULT_VALUE_INTEGER;
	f->_value_float		= TAB_DEFAULT_VALUE_FLOAT;
	f->_value_string	= 0;

	if (!val)
	{
		f->_value_string = malloc (sizeof (char) * strlen (TAB_DEFAULT_VALUE_STRING));
		if (!f->_value_string)
			return false;
	}
	else
	{
		f->_value_string = malloc (sizeof (char) * strlen (val));
		if (!f->_value_string)
			return false;
		strcpy(f->_value_string, val);

		p = _tab_parse_value (val);

		f->_datatype			= p->_datatype;

		if (f->_datatype == TAB_FLOAT || f->_datatype == TAB_INTEGER)
			f->_value_float		= p->_value_float;

		if (f->_datatype == TAB_INTEGER)
			f->_value_integer	= p->_value_integer;
	}

	free(p);
	return true;
}

bool
tab_field_terminate (tab_field_t* f)
{
	if (f->_value_string)
		free (f->_value_string);

	return true;
}

tab_field_t*
tab_field_create (const char* val)
{
	tab_field_t* f;

	f = malloc (sizeof (tab_field_t));

	if (f)
		tab_field_initialize (f, 0);

	return f;
}

void
tab_field_destroy (tab_field_t* f)
{
	tab_field_terminate (f);
	free (f);
}

tab_dt_t
tab_field_get_datatype (const tab_field_t* f)
{
	return f->_datatype;
}

bool
tab_field_set_datatype (tab_field_t* f, tab_dt_t dt)
{
	return true;
}

long int
tab_field_get_integer_value (const tab_field_t* f)
{
	return f->_value_integer;
}

bool
tab_field_set_integer_value (tab_field_t* f, const long int ival)
{
	return true;
}

double
tab_field_get_float_value (const tab_field_t* f)
{
	return f->_value_float;
}

bool
tab_field_set_float_value (tab_field_t* f, const double fval)
{
	return true;
}

const char *
tab_field_get_string_value (const tab_field_t* f)
{
	return f->_value_string;
}

bool
tab_field_set_string_value (tab_field_t* f, const char* sval)
{
	tab_field_t	*p;

	if (f->_value_string)
		free (f->_value_string);

	f->_value_string = malloc (sizeof (char) * strlen (sval));
	if (!f->_value_string)
		return false;
	strcpy(f->_value_string, sval);

	p = _tab_parse_value (sval);

	f->_datatype			= p->_datatype;

	if (f->_datatype == TAB_INTEGER)
	{
		f->_value_float		= p->_value_float;
		f->_value_integer	= p->_value_integer;
	}

	else if (f->_datatype == TAB_FLOAT)
	{
		f->_value_float		= p->_value_float;
		f->_value_integer	= TAB_DEFAULT_VALUE_INTEGER;
	}

	else
	{
		f->_value_float		= TAB_DEFAULT_VALUE_FLOAT;
		f->_value_integer	= TAB_DEFAULT_VALUE_INTEGER;
	}

	free(p);
	return true;
}

tab_field_t*
_tab_parse_value (const char * val)
{
	tab_field_t*	f;
	size_t				sz;
	size_t				cx;
	int						state;
	char					c;

	int						val_sign;
	long int			val_whole;
	long int			val_frac;
	int						exp_sign;
	long int			exp_whole;

	f							= tab_field_create (0);
	f->_datatype	= TAB_INTEGER;
	sz						= strlen (val);

	#if (DEBUG == 1)
		printf("Parsing value '%s':\n", val);
	#endif

	if (sz <= 0)
		f->_datatype = TAB_STRING;
	else
	{
		state			= 0;
		val_sign	= 1;
		val_whole	= 0;
		val_frac	= 0;
		exp_sign	= 1;
		exp_whole	= 0;

		for (cx=0; cx<sz; cx++)
		{
			c = val[cx];

			#if (DEBUG == 1)
				printf("- State %d, C=%c", state, c);
			#endif

			// state=0 : no character encountered yet
			if (state == 0)
			{
				if (c == '+')
				{
					state			= 1;
					val_sign	= 1;
				}
				else if (c == '-')
				{
					state			= 1;
					val_sign	= -1;
				}
				else if (c >= '0' && c <= '9')
				{
					state			= 1;
					val_whole = (c - '0');
				}
				else if (c == '.')
				{
					state			= 2;
					f->_datatype = TAB_FLOAT;
				}
				else
				{
					f->_datatype = TAB_STRING;
					break;
				}
			}

			// state=1 : integer part
			else if (state == 1)
			{
				if (c >= '0' && c <= '9')
				{
					val_whole *= 10;
					val_whole += (c - '0');
				}
				else if (c == '.')
				{
					state     = 2;
					f->_datatype = TAB_FLOAT;
				}
				else if (c == 'e' || c == 'E')
				{
					state			= 3;
				}
				else
				{
					f->_datatype = TAB_STRING;
					break;
				}
			}

			// state=2 : fractional part
			else if (state == 2)
			{
				if (c >= '0' && c <= '9')
				{
					val_frac *= 10;
					val_frac += (c - '0');
				}
				else if (c == 'e' || c == 'E')
				{
					state			= 3;
				}
				else
				{
					f->_datatype = TAB_STRING;
					break;
				}
			}

			// state=3 : expecting exponent
			else if (state == 3)
			{
				if (c == '+')
				{
					state			= 4;
					exp_sign	= 1;
				}
				else if (c == '-')
				{
					state			= 4;
					exp_sign	= -1;
					f->_datatype = TAB_FLOAT;
				}
				else if (c >= '0' && c <= '9')
				{
					state			= 4;
					exp_whole	= (c - '0');
				}
				else
				{
					f->_datatype = TAB_STRING;
					break;
				}
			}

			// state=4 : exponent whole part
			else if (state == 4)
			{
				if (c >= '0' && c <= '9')
				{
					exp_whole	*= 10;
					exp_whole += (c - '0');
				}
				else
				{
					f->_datatype = TAB_STRING;
					break;
				}
			}

			// Somehow we are in a bad state
			else
			{
				f->_datatype = TAB_STRING;
				break;
			}

			#if (DEBUG == 1)
				printf(" [val_sign=%d,val_whole=%ld,val_frac=%ld,exp_sign=%d,exp_whole=%d]\n", val_sign, val_whole, val_frac, exp_sign, exp_whole);
			#endif
		}

		if (f->_datatype == TAB_INTEGER)
		{
			f->_value_integer = val_sign * val_whole * pow (10, exp_whole);
			f->_value_float		= f->_value_integer;
		}
		else if (f->_datatype == TAB_FLOAT)
		{
			f->_value_float		= val_frac;
			while (f->_value_float >= 1)
				f->_value_float /= 10;
			f->_value_float += val_whole;
			f->_value_float *= pow (10, exp_whole);
			if (exp_sign < 0)
				f->_value_float = 1/f->_value_float;
			f->_value_float *= val_sign;
		}
	}

	return f;
}

void
tab_field_print_details (const tab_field_t* f)
{
	printf("Tab Field %p\n", f);
	printf("  Integer Value: %ld\n", f->_value_integer);
	printf("  Float Value: %lf\n", f->_value_float);
	printf("  String Value: '%s'\n", f->_value_string);
}

