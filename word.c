#include <stdio.h>
#include "word.h"


static CELL  cell[MAX_CELLS_SIZE] = { 0 };
static CELL* const nil = &cell[0];

static CELL* top_of_free_cells;
static CELL* top_of_para_stack;
static CELL* top_of_cntl_stack;
static CELL* top_of_atom_list;
static CELL* top_of_name_list;

static ERR intialize_atoms(void);


ERR initialize_cells(void)
{
	int32_t idex;
	int32_t next_idex;

	for( idex = 1, next_idex = 2; idex < MAX_CELLS_SIZE; idex++, next_idex++ )
	{
		if( next_idex < MAX_CELLS_SIZE )
		{
			cell[idex].cdr.idex = next_idex;
		}
	}

	top_of_free_cells = &cell[1];
	top_of_para_stack = nil;
	top_of_cntl_stack = nil;
	top_of_atom_list = nil;
	top_of_name_list = nil;

	intialize_atoms();
	
	return ERR_OK;
}

CELL* idex2cellp(index_t idex)
{
	if( idex < 0 || MAX_CELLS_SIZE <= idex )
	{
		return nil;
	}
	return &cell[idex];
}

index_t cellp2idex(CELL* p_cell)
{
	index_t ret;
	
	if( p_cell < &cell[0] )
	{
		return NONE;
	}
	
	ret =  (index_t)(p_cell - (&cell[0]));

	if( ret >= MAX_CELLS_SIZE )
	{
		return NONE;
	}
	return ret;
}

CELL* alloc_cell(void)
{
	
	CELL* p_cell;
	
	if( top_of_free_cells == nil )
	{
		p_cell =  nil;
	}
	else
	{
		p_cell = top_of_free_cells;
		top_of_free_cells = idex2cellp(p_cell->cdr.idex);
		p_cell->cdr.idex = 0;
	}

	return p_cell;
}

BOOL is_same_cell(CELL* p_cell_a, CELL* p_cell_b)
{
	if( p_cell_a == p_cell_b )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL  is_nil(CELL* p_cell)
{
	return is_same_cell(p_cell, nil);
}

TYPE type_of_cell(CELL* p_cell)
{
	if( ATOM_TYPE == (p_cell->car.type & TYPE_MASK) )
	{
		return ATOM;
	}
	else
	{
		if( NAME_TYPE == (p_cell->cdr.type & TYPE_MASK) )
		{
			return NAME;
		}
		return LIST;
	}
}
	
CELL* car_of_cell(CELL* p_cell)
{
	if( ATOM == type_of_cell(p_cell) )
	{
		return nil;
	}
	return idex2cellp(p_cell->car.idex);
}

CELL* cdr_of_cell(CELL* p_cell)
{
	if( ATOM == type_of_cell(p_cell) )
	{
		return nil;
	}
	return idex2cellp(p_cell->cdr.idex);
}

CELL* cons_with_cell(CELL* p_elm, CELL* p_cell)
{
	CELL* p_new_cell;

	p_new_cell = alloc_cell();
	if( FALSE == is_nil(p_new_cell) )
	{
		p_new_cell->car.idex = cellp2idex(p_elm);
		p_new_cell->cdr.idex = cellp2idex(p_cell);
	}
	return p_new_cell;
}

ERR push_cell(CELL* p_elm)
{
	CELL* p_new_cell;

	p_new_cell = alloc_cell();
	if( is_nil(p_new_cell) )
	{
		return ERR_NG;
	}
	
	p_new_cell->car.idex = cellp2idex(p_elm);
	p_new_cell->cdr.idex = cellp2idex(top_of_para_stack);
	top_of_para_stack = p_new_cell;

	return ERR_OK;
}

CELL* pop_cell(void)
{
	CELL* p_cell;

	p_cell = top_of_para_stack;
	top_of_para_stack = idex2cellp(p_cell->cdr.idex);

	return idex2cellp(p_cell->car.idex);
}

ERR as_atom(CELL* p_cell, int32_t wnum, int32_t data)
{
	if( wnum & TYPE_MASK )
	{
		return ERR_NG;
	}
	
	p_cell->car.type = ATOM_TYPE;
	p_cell->car.wnum |= (REST_MASK & wnum);
	p_cell->cdr.data = data;

	return ERR_OK;
}

ERR as_name(CELL* p_cell)
{
	TYPE type;

	type = type_of_cell(p_cell);
	
	switch(type)
	{
	case ATOM:
		return ERR_NG;
	case NAME:
		return ERR_NG;
	case LIST:
		p_cell->cdr.type |= NAME_TYPE;
		return ERR_OK;
	default:
		return ERR_NG;
	}
}

/* static */
static CELL* construct_atom(int32_t wnum, int32_t data)
{
	ERR err;
	CELL* p_cell;
	CELL* p_atom;

	p_atom = alloc_cell();
	if( TRUE == is_nil(p_atom) )
	{
		return nil;
	}

	err = as_atom(p_atom, wnum, data);
	if( ERR_NG == err )
	{
		return nil;
	}

	p_cell = cons_with_cell(p_atom, top_of_atom_list);
	if( TRUE == is_nil(p_cell) )
	{
		return nil;
	}

	top_of_atom_list = p_cell;

	return p_cell;
}

static ERR intialize_atoms(void)
{
	ERR err = ERR_OK;
	CELL* p_cell;
	
	p_cell = construct_atom(VALUE, '0'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '1'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '2'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '3'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '4'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '5'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '6'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '7'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '8'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '9'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;

	p_cell = construct_atom(VALUE, 'a'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'b'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'c'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'd'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'e'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'f'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'g'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'h'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'i'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'j'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'k'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'l'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'm'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'n'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'o'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'q'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'r'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 's'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 't'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'u'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'v'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'w'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'x'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'y'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'z'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;

	p_cell = construct_atom(VALUE, 'A'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'B'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'C'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'D'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'E'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'F'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'G'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'H'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'I'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'J'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'K'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'L'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'M'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'N'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'O'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'P'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'R'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'S'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'T'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'U'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'V'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'W'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'X'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'Y'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, 'Z'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;

	p_cell = construct_atom(VALUE, ':'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '['); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, ']'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '{'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '}'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '('); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, ')'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '+'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '-'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '*'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '/'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '%'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '='); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '<'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '>'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '!'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '#'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '$'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '&'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '|'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, ','); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '.'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '^'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '_'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, '@'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;
	p_cell = construct_atom(VALUE, ';'); err |= is_nil(p_cell) ? ERR_NG : ERR_OK;

	return err;
}


void dump_cells(void)
{
	index_t i;
	TYPE type;

	printf("top of free cells = %d\n", cellp2idex(top_of_free_cells));
	printf("top of para stack = %d\n", cellp2idex(top_of_para_stack));
	printf("top of cntl stack = %d\n", cellp2idex(top_of_cntl_stack));
	printf("top of atom list = %d\n", cellp2idex(top_of_atom_list));
	printf("top of name list = %d\n", cellp2idex(top_of_name_list));
	
	for( i = 0; i < MAX_CELLS_SIZE; i++ )
	{
		type = type_of_cell(&cell[i]);
		if( ATOM == type )
			printf("idex=%d ATOM car:%x cdr:%d\n", i, cell[i].car.idex, cell[i].cdr.idex);
		if( LIST == type )
			printf("idex=%d LIST car:%d cdr:%d\n", i, cell[i].car.idex, cell[i].cdr.idex);
		if( NAME == type )
			printf("idex=%d NAME car:%d cdr:%d\n", i, cell[i].car.idex, cell[i].cdr.idex);
	}
}
