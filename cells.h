#ifndef _CELL_HEADER_
#define _CELL_HEADER_

/* construction of cell
 *         +---+---+
 *  cell = |car|cdr|
 *         +---+---+
 *
 *         +-+-------------------------------+
 *  car  = |A|                               |
 *         +-+-------------------------------+
 *          A:Type of ATOM
 *         +-+-------------------------------+
 *  cdr  = |N|                               |
 *         +-+-------------------------------+
 *          N:Type of NAME
 *
 *         +-+-------------------------------+-+-------------------------------+
 *  ATOM = |A| wnum                          | | data                          |
 *         +-+-------------------------------+-+-------------------------------+
 *          
 *         +-+-------------------------------+-+-------------------------------+
 *  LIST = | | idex                          | | idex                          |
 *         +-+-------------------------------+-+-------------------------------+
 *          
 *         +-+-------------------------------+-+-------------------------------+
 *  NAME = | | idex for ATOM                 |N| idex for LIST (NAME)          |
 *         +-+-------------------------------+-+-------------------------------+
 *          
 */

/* cells size */
#define MAX_CELLS_SIZE (32 * 100)

/* MASK for cell */
#define TYPE_MASK (0x80000000)
#define REST_MASK (0x7FFFFFFF)

/* Type of ATOM */
#define ATOM_TYPE TYPE_MASK
/* Type of NAME */
#define NAME_TYPE TYPE_MASK

/* Ileagal Index Number  */
#define NONE_I (CELL_I)(-1)
/* Ileagal Pointer */
#define NONE_P (CELL_P)(-1)
/* NONE CELLS */
#define NONE_CELLS (CELLS*)(0)

typedef union car {
	int32_t type;
	int32_t idex;
	int32_t wnum;
} CAR;

typedef union cdr {
	int32_t type;
	int32_t idex;
	int32_t data;
} CDR;
  
typedef struct cell {
	CAR car;
	CDR cdr;
} CELL;

typedef int32_t CELL_I;

typedef CELL* CELL_P;

typedef struct cells {
	int32_t size;
	CELL    cell[MAX_CELLS_SIZE];
	CELL_P  top_of_free_cells;
	CELL_P  top_of_para_stack;
	CELL_P  top_of_cntl_stack;
	CELL_P  top_of_dict_list;
	CELL_P  top_of_name_list;
	int32_t map[MAX_CELLS_SIZE / 32];
} CELLS;

typedef enum {
	NONE,
	LIST,
	NAME,
	ATOM,
} TYPE;

typedef enum {
	FALSE,
	TRUE,
} BOOL;



#if 0
/*******************/
/* global function */
/*******************/
/* initialize */
extern CELLS* cells_initialize(CELLS* cels);
/* garbage collection */
extern CELLS* cells_garbage_collection(CELLS* cells);
/* functions of cell */
extern CELL_P cells_alloc(CELLS* cells);
extern CELL_P cells_nil(CELLS* cells);
extern CELL_P cell_as_atom(CELL_P p_cell, int32_t wnum, int32_t data);
extern CELL_P cell_as_name(CELL_P p_cell);
/* functions of list */
extern CELL_P cells_head(CELLS* cells, CELL_P p_cell);
extern CELL_P cells_tail(CELLS* cells, CELL_P p_cell);
extern CELL_P cells_push(CELLS* cells, CELL_P p_elm, CELL_P p_list);
extern BOOL cells_equal(CELLS* cells, CELL_P p_list_a, CELL_P p_list_b);



/* debug */
extern void cells_dump(CELLS* cells);
#endif


#endif /* _CELL_HEADER_ */
