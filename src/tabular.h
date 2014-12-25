#ifndef _TABULAR_H_
#define _TABULAR_H_

struct	tab_table;

enum tab_type
{
	TAB_INT,
	TAB_FLOAT,
	TAB_STRING
};

struct tab_table* tab_table_new			(void);
void							tab_table_delete	(struct tab_table*);

#endif /* _TABULAR_H_ */

