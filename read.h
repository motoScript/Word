#ifndef _READ_HEADER_
#define _READ_HEADER_

#define MAX_INPUT_BUF_SIZE 256

typedef struct read {
	char read_buff[MAX_INPUT_BUF_SIZE];
	int32_t idx;
	int32_t cnt;
} READ_INFO;

extern void read_initialize(READ_INFO* read_info);




#endif
