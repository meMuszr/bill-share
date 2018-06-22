#include <stdio.h>
#include <string.h>
#include "includes/bill-share.h"
void iterator(void *ptr, size_t size_of_elements, size_t number_of_elements)
{
	void *end = ptr + (size_of_elements * number_of_elements);
	for (; ptr != end; ptr += size_of_elements)
	{
		printf("%d\n", *((int *)ptr));
	}
}
int main(void)
{
	unsigned char type;
	char string_input_buffer[256];
	init();
	while (1)
	{
		puts("insert type\n"
			 "0: new unit\n"
			 "1: new tenant\n"
			 "2: get units\n"
			 "3: get tenants\n");
		scanf("%hhu", &type);
		switch (type)
		{
		case 0:
			puts("New unit address: ");
			scanf("%256s\n", string_input_buffer);
			insert_unit(string_input_buffer);
			break;
		case 2:
		{
			struct unit result_row = {};
			struct sqlite3_stmt *stmt = NULL;
			struct unit results_buffer[100];
			int i = 0;
			while ((stmt = get_units(&stmt,&result_row)) != NULL)
			{
				results_buffer[i++] = result_row;
			}
			struct unit results[i];
			memcpy(results,results_buffer,sizeof(struct unit) * i);
				printf("id: %u, name: %s\n", result_row.id, result_row.name);
		}
		}
	}
	close();
	return 0;
}
