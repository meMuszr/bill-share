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
void remove_new_line(char *str) {
				while(str && *str != '\n') str++;
				*str = '\0';
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
																"3: get tenants\n"
																"4: get receipts\n"
																"5: new receipt\n"
																"6: calculate apartment monthly costs\n");

								if (fgets(string_input_buffer, sizeof(string_input_buffer), stdin) != NULL)
								{
												sscanf(string_input_buffer, "%hhu", &type);
								}
								switch (type)
								{
												case 0:
																{
																				puts("New unit address: ");
																				if (fgets(string_input_buffer, sizeof(string_input_buffer), stdin) != NULL)
																				{
																								remove_new_line(string_input_buffer);
																								insert_unit(string_input_buffer);
																				}
																}
																break;
												case 1:
																{
																				puts("New tenant: ");
																				if (fgets(string_input_buffer, sizeof(string_input_buffer), stdin) != NULL)
																				{
																								char tenant_name[249];
																								//reuse type for unit id
																								sscanf(string_input_buffer, "%hhu %254c", &type, tenant_name);
																								remove_new_line(tenant_name);
																								insert_tenant(type,tenant_name);
																				}
																}
																break;
												case 2:
																{
																				struct unit result_row = {};
																				struct sqlite3_stmt *stmt = NULL;
																				struct unit results_buffer[100];
																				int i = 0;
																				while ((stmt = get_units(&stmt, &result_row)) != NULL)
																				{
																								results_buffer[i++] = result_row;
																								printf("%u %s\n", result_row.id, result_row.name);
																				}
																}
																break;
												case 3:
																{
																				struct tenant result_row = {};
																				struct sqlite3_stmt *stmt = NULL;
																				struct tenant results_buffer[100];
																				int i = 0;
																				while((stmt = get_tenants(&stmt,&result_row)) != NULL)
																				{
																								results_buffer[i++] = result_row;
																								printf("%u %u %s\n",result_row.id,result_row.unit_id,result_row.name);
																				}
																}
																break;
												case 4:
																{
																				struct receipt result_row = {};
																				struct sqlite3_stmt *stmt = NULL;
																				struct receipt results_buffer[100];
																				int i = 0;
																				while((stmt = get_receipts(&stmt,&result_row)) != NULL) 
																				{
																								results_buffer[i++] = result_row;
																								printf("%u %s %u, %.2f, %u\n",result_row.id,result_row.name,result_row.tenant_id,(float)result_row.cost / 100,result_row.date);
																				}
																}
																break;
												case 5:
																{
																				puts("New receipt: ");
																				if (fgets(string_input_buffer, sizeof(string_input_buffer), stdin) != NULL)
																				{
																								char receipt_name[249];
																								unsigned int cost;
																								time_t date;
																								//reuse type for tenant id
																								sscanf(string_input_buffer, "%hhu %u %lu %254c\n", &type, &cost, &date, receipt_name);
																								remove_new_line(receipt_name);
																								insert_receipt(receipt_name,type,cost,date);
																				}
																}
																break;
												default: {
																				 close();
																				 return 0;
																 }
								}
				}
				close();
				return 0;
}
