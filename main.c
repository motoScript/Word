#include <stdio.h>
#include "word.h"

void test_print(CELL* p_cell)
{
	if( is_nil(p_cell) == TRUE )
		printf("cell is nill(%d)\n", cellp2idex(p_cell));
	else
		printf("cell is not nil : %d\n", cellp2idex(p_cell));	
}

int main(void)
{
	ERR ret;
	CELL* cell;
	CELL* cell_a;
	CELL* cell_b;

	ret = initialize_cells();
	printf("ret = %d\n", ret);

	cell_a = alloc_cell();
	test_print(cell_a);
	
	cell_b = alloc_cell();
	test_print(cell_b);
	
	push_cell(cell_a);
	push_cell(cell_b);
	push_cell(cell_a);
	
	cell = pop_cell();
	test_print(cell);

	dump_cells();
	
	return 0;
}
