#include <stdio.h>
#include "word.h"
#include "read.h"

void read_initialize(READ_INFO* read_info)
{
	int32_t idx;

	for( idx = 0; idx < MAX_INPUT_BUF_SIZE; idx++ )
	{
		read_info->read_buff[idx] = 0;
	}

	read_info->idx = 0;
	read_info->cnt = 0;
}

int32_t read_line(READ_INFO* read_info)
{
	int32_t chr;
	int32_t idx = 0;

	while( (chr = getchar()) != EOF )
	{
		if( idx == MAX_INPUT_BUF_SIZE - 1 )
		{
			break;
		}
		input_buf[idx++] = (char)chr;
	}
	input_buf[idx] = '\0';

	return idx;
}

