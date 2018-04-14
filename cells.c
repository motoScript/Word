#include <stdio.h>
#include <stdlib.h>
#include "cells.h"

/* static function declare */
static void initialize(CELLS* cells);
static void set_all_marks_of_map(CELLS* cells);
static void clr_mark_of_map(CELLS* cells, CELL_P p_cell);
static void garbage_collection(CELLS* cells);
static CELL_P index2cellp(CELLS* cells, CELL_I index);
static CELL_I cellp2index(CELLS* cells, CELL_P p_cell);
static CELL_P alloc(CELLS* cells);
static CELL_P nil(CELLS* cells);
static CELL_P car(CELLS* cells, CELL_P p_cell);
static CELL_P cdr(CELLS* cells, CELL_P p_cell);
static BOOL is_nil(CELLS* cells, CELL_P p_cell);
static TYPE type(CELL_P p_cell);
static CELL_P as_atom(CELL_P p_cell, int32_t wnum, int32_t data);
static CELL_P as_name(CELL_P p_cell);
static CAR get_wnum(CELL_P p_cell);
static CDR get_data(CELL_P p_cell);
static BOOL eq(CELL_P p_cell_a, CELL_P p_cell_b);
static BOOL eqval(CELL_P p_cell_a, CELL_P p_cell_b);
static CELL_P cons(CELLS* cells, CELL_P p_elm, CELL_P p_list);
static CELL_P connect(CELLS* cells, CELL_P p_cell, CELL_P p_list);
static BOOL equal(CELLS* cells, CELL_P p_list_a, CELL_P p_list_b);
static CELL_P pickup_name(CELLS* cells, CELL_P p_name);
static CELL_P pickup_asoc(CELLS* cells, CELL_P p_name, CELL_P p_dict);
static CELL_P search_asoc(CELLS* cells, CELL_P p_name);
static CELL_P reverse(CELLS* cells, CELL_P p_cell);

/* initialize */
static void initialize(CELLS* cells)
{
	CELL_I idex;
	CELL_I next_idex;

	for( idex = 1, next_idex = 2; idex < MAX_CELLS_SIZE; idex++, next_idex++ )
	{
		if( next_idex < MAX_CELLS_SIZE )
		{
			cells->cell[idex].cdr.idex = next_idex;
		}
	}

	cells->top_of_free_cells = &cells->cell[1];
}

/* garbage collection */
static void set_all_marks_of_map(CELLS* cells)
{
	int32_t idex;
	int32_t size = MAX_CELLS_SIZE / 32;

	for(idex = 0; idex < size; idex++)
	{
		cells->map[idex] = 0xffffffff;
	}
}

static void clr_mark_of_map(CELLS* cells, CELL_P p_cell)
{
	CELL_I idex;
	int32_t map_index;
	int32_t bit_index;

	idex = (CELL_I)(p_cell - &cells->cell[0]);
	map_index = (int32_t)(idex / 32);
	bit_index = (int32_t)(idex % 32);
	cells->map[map_index] &= ~(1 << bit_index);
}

static void garbage_collection(CELLS* cells)
{
	CELL_I index;
	CELL_P p_cell;
	CELL_P p_free;
	int32_t map_index;
	int32_t bit_index;
	int32_t check;

	cells->top_of_free_cells = nil(cells);
	for( map_index = 0; map_index < (MAX_CELLS_SIZE / 32); map_index++ )
	{
		for( bit_index = 0; bit_index < 32; bit_index++ )
		{
			check = cells->map[map_index] & (1 << bit_index);
			if(check != 0)
			{
				index = map_index * 32 + bit_index;
				p_cell = index2cellp(cells, index);
				if(p_cell != NONE_P)
				{
					p_cell->car.idex = 0;
					p_cell->cdr.idex = 0;
					p_free = cells->top_of_free_cells;
					cells->top_of_free_cells = connect(cells, p_cell, p_free);
				}
			}
		}
	}
}

/* convert */
static CELL_P index2cellp(CELLS* cells, CELL_I index)
{
	if( index < 0 || MAX_CELLS_SIZE <= index )
	{
		return NONE_P;
	}
	return &cells->cell[index];
}

static CELL_I cellp2index(CELLS* cells, CELL_P p_cell)
{
	CELL_I index;
	
	if( p_cell < &cells->cell[0] )
	{
		return NONE_I;
	}
	
	index =  (CELL_I)( p_cell - &cells->cell[0] );

	if( index >= MAX_CELLS_SIZE )
	{
		return NONE_I;
	}
	return index;
}

/* functions of cell */
static CELL_P alloc(CELLS* cells)
{
	CELL* p_cell;
	
	if(TRUE == is_nil(cells, cells->top_of_free_cells) )
	{
		p_cell = NONE_P;
	}
	else
	{
		p_cell = cells->top_of_free_cells;
		cells->top_of_free_cells = cdr(cells, p_cell);
		p_cell->cdr.idex = 0;
	}

	return p_cell;
}

static CELL_P nil(CELLS* cells)
{
	return &cells->cell[0];
}

static CELL_P car(CELLS* cells, CELL_P p_cell)
{
	if( ATOM == type(p_cell) )
	{
		return NONE_P;
	}
	return index2cellp(cells, p_cell->car.idex);
}
	
static CELL_P cdr(CELLS* cells, CELL_P p_cell)
{
	if( ATOM == type(p_cell) )
	{
		return NONE_P;
	}
	return index2cellp(cells, p_cell->cdr.idex);
}

static BOOL is_nil(CELLS* cells, CELL_P p_cell)
{
	return eq(nil(cells), p_cell);
}

static TYPE type(CELL_P p_cell)
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

static CELL_P as_atom(CELL_P p_cell, int32_t wnum, int32_t data)
{
	switch(type(p_cell))
	{
	case ATOM:
		return NONE_P;
	case NAME:
	case LIST:
		p_cell->car.type = ATOM_TYPE;
		p_cell->car.wnum |= (REST_MASK & wnum);
		p_cell->cdr.data = data;
		return p_cell;
	default:
		return NONE_P;
	}
}

static CELL_P as_name(CELL_P p_cell)
{
	switch(type(p_cell))
	{
	case ATOM:
		return NONE_P;
	case NAME:
	case LIST:
		p_cell->cdr.type |= NAME_TYPE;
		return p_cell;
	default:
		return NONE_P;
	}	
}

static CAR get_wnum(CELL_P p_cell)
{
	return (CAR)p_cell->car;
}

static CDR get_data(CELL_P p_cell)
{
	return (CDR)p_cell->cdr;
}

static BOOL eq(CELL_P p_cell_a, CELL_P p_cell_b)
{
	return (p_cell_a == p_cell_b) ? TRUE : FALSE;
}

static BOOL eqval(CELL_P p_cell_a, CELL_P p_cell_b)
{
	if( p_cell_a->car.wnum != p_cell_b->car.wnum )
	{
		return FALSE;
	}

	if( p_cell_a->cdr.data != p_cell_b->cdr.data )
	{
		return FALSE;
	}

	return TRUE;
}

/* functions of list */
static CELL_P cons(CELLS* cells, CELL_P p_elm, CELL_P p_list)
{
	CELL_P p_new_cell;

	p_new_cell = alloc(cells);
	if( NONE_P == p_new_cell )
	{
		return NONE_P;
	}

	p_new_cell->car.idex = cellp2index(cells, p_elm);
	p_new_cell->cdr.idex = cellp2index(cells, p_list);

	return p_new_cell;
}

static CELL_P connect(CELLS* cells, CELL_P p_cell, CELL_P p_list)
{
	p_cell->cdr.idex = cellp2index(cells, p_list);
	return p_cell;
}

static BOOL equal(CELLS* cells, CELL_P p_list_a, CELL_P p_list_b)
{
	CELL_P pa = p_list_a;
	CELL_P pb = p_list_b;

	while( FALSE == is_nil(cells, pa) )
	{
		CELL_P elm_a;
		CELL_P elm_b;
		
		elm_a = car(cells, pa);
		elm_b = car(cells, pb);
		if( FALSE == eqval(elm_a, elm_b) )
		{
			return FALSE;
		}
		pa = cdr(cells, pa);
		pb = cdr(cells, pb);
	}
	if( TRUE == is_nil(cells, pb) )
	{
		return TRUE;
	}
	return FALSE;
}
				
static CELL_P pickup_name(CELLS* cells, CELL_P p_name)
{
	CELL_P pl = cells->top_of_name_list;
	CELL_P pa = p_name;	
	CELL_P pb;

	while( FALSE == is_nil(cells, pl) )
	{
		pb = car(cells, pl);
		pl = cdr(cells, pl);
		if( TRUE == equal(cells, pa, pb) )
		{
			return pb;
		}
	}

	pl = cons(cells, pa, pl);
	cells->top_of_name_list = pl;
	return pa;
}

static CELL_P pickup_asoc(CELLS* cells, CELL_P p_name, CELL_P p_dict)
{
	CELL_P pd = p_dict;
	CELL_P pa = p_name;
	CELL_P pb;
	CELL_P pasoc;

	while( FALSE == is_nil(cells, pd) )
	{
		pasoc = car(cells, pd);
		pd = cdr(cells, pd);
		pb = cdr(cells, pasoc);
		if( TRUE == equal(cells, pa, pb) )
		{
			return pasoc;
		}
	}

	return NONE_P;		
}

static CELL_P search_asoc(CELLS* cells, CELL_P p_name)
{
	CELL_P pl = cells->top_of_dict_list;
	CELL_P pa = p_name;
	CELL_P pd;
	CELL_P pasoc;

	while( FALSE == is_nil(cells, pl) )
	{
		pd = car(cells, pl);
		pl = cdr(cells, pl);
		pasoc = pickup_asoc(cells, pa, pd);
		if( NONE_P != pasoc )
		{
			return pasoc;
		}
	}

	return NONE_P;
}

static CELL_P reverse(CELLS* cells, CELL_P p_cell)
{
	CELL_P p_src;
	CELL_P p_dst;
	CELL_P p_elm;

	p_src = p_cell;
	p_dst = nil(cells);

	while( FALSE == is_nil(cells, p_src ) )
	{
		p_elm = car(cells, p_src);
		p_src = cdr(cells, p_src);
		p_dst = cons(cells, p_elm, p_dst);
	}

	return p_dst;
}

