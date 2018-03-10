#include <stdint.h>

#define MAX_CELLS_SIZE 200

/* MASK */
#define TYPE_MASK 0x80000000
#define REST_MASK 0x7FFFFFFF

/* Type */
#define ATOM_TYPE TYPE_MASK
#define NAME_TYPE TYPE_MASK

#define nil (CELL*)0
#define NONE (index_t)(-1)

typedef int32_t index_t;

typedef enum {
	FALSE,
	TRUE,
} BOOL;

typedef enum {
	ERR_OK,
	ERR_NG,
} ERR;

typedef enum {
	LIST,
	NAME,
	ATOM,
} TYPE;

typedef union car {
	int32_t type;
	index_t idex;
	int32_t wnum;
} CAR;

typedef union cdr {
	int32_t type;
	index_t idex;
	int32_t data;
} CDR;
  
typedef struct cell {
  CAR car;
  CDR cdr;
} CELL;

enum prime_function {
	VALUE,
};

extern ERR initialize_cells(void);
extern CELL* idex2cellp(index_t idex);
extern index_t cellp2idex(CELL* p_cell);
extern CELL* alloc_cell(void);
extern BOOL is_same_cell(CELL* p_cell_a, CELL* p_cell_b);
extern BOOL is_nil(CELL* p_cell);
extern TYPE type_of_cell(CELL* p_cell);
extern CELL* car_of_cell(CELL* p_cell);
extern CELL* cdr_of_cell(CELL* p_cell);
extern CELL* cons_with_cell(CELL* p_elm, CELL* p_cell);
extern ERR push_cell(CELL* p_elm);
extern CELL* pop_cell(void);
extern ERR as_atom(CELL* p_cell, int32_t wnum, int32_t data);
extern ERR as_name(CELL* p_cell);
extern void dump_cells(void);

