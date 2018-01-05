typedef union car {
  int index;
  int bits;
} CAR;

typedef union cdr {
  int index;
  int prime;
} CDR;
  

typedef struct cell {
  CAR car;
  CDR cdr;
} CELL;

