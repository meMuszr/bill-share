#include <stdio.h>
#include <sqlite3.h>
#include "includes/db_statements.h"
#include "includes/bill-share.h"

static struct sqlite3 *db = NULL;
static struct sqlite3_stmt *prepared_stmt = NULL;
static char *errorMsg = NULL;

static void print_error_msg(char **errorMsg, int free)
{
				char *pErrorMsg = *errorMsg;
				if (pErrorMsg == NULL)
				{
								return;
				}
				puts(pErrorMsg);
				if (free > 0)
				{
								sqlite3_free(pErrorMsg);
				}
}

static void exec_sql(const char *stmt)
{
				sqlite3_exec(db, stmt, NULL, NULL, &errorMsg);
				print_error_msg(&errorMsg, 1);
}

sqlite3_int64 insert_unit(const char *name)
{
				sqlite3_prepare_v2(db, INSERT_NEW_UNIT_STATEMENT, -1, &prepared_stmt, NULL);

				sqlite3_bind_text(prepared_stmt, 1, name, -1, SQLITE_STATIC);

				if (sqlite3_step(prepared_stmt) == SQLITE_DONE)
				{
								return sqlite3_last_insert_rowid(db);
				}
				else
				{
								return -1;
				}

				sqlite3_clear_bindings(prepared_stmt);
				sqlite3_finalize(prepared_stmt);
}
struct sqlite3_stmt *get_units(struct sqlite3_stmt **units_prepared_stmt, struct unit *outUnit)
{
				if (*units_prepared_stmt == NULL)
				{
								sqlite3_prepare_v2(db, SELECT_UNITS_QUERY, -1, units_prepared_stmt, NULL);
				}
				struct sqlite3_stmt *pUnits_prepared_stmt = *units_prepared_stmt;
				int rc = 0;
				if ((rc = sqlite3_step(pUnits_prepared_stmt)) == SQLITE_ROW)
				{
								outUnit->id = sqlite3_column_int(pUnits_prepared_stmt, 0);
								outUnit->name = sqlite3_column_text(pUnits_prepared_stmt, 1);
								return pUnits_prepared_stmt;
				}
				sqlite3_finalize(pUnits_prepared_stmt);
				return NULL;
}

struct sqlite3_stmt *get_tenants(struct sqlite3_stmt **tenants_prepared_stmt, struct tenant *outTenant) {
				if(*tenants_prepared_stmt == NULL) {
								sqlite3_prepare_v2(db,SELECT_TENANTS_QUERY,-1,tenants_prepared_stmt,NULL);
				}
				struct sqlite3_stmt *pTenants_prepared_stmt = *tenants_prepared_stmt;
				int rc = 0;
				if((rc = sqlite3_step(pTenants_prepared_stmt)) == SQLITE_ROW) {
								outTenant->id = sqlite3_column_int(pTenants_prepared_stmt,0);
								outTenant->unit_id = sqlite3_column_int(pTenants_prepared_stmt,1);
								outTenant->name = sqlite3_column_text(pTenants_prepared_stmt,2);
								return pTenants_prepared_stmt;
				}
				sqlite3_finalize(pTenants_prepared_stmt);
				return NULL;
}

struct sqlite3_stmt *get_tenants_by_unit(unsigned int unitId)
{
				struct sqlite3_stmt *tenants_prepared_stmt = NULL;
				sqlite3_prepare_v2(db, SELECT_TENANTS_QUERY, -1, &tenants_prepared_stmt, NULL);
				sqlite3_bind_int64(tenants_prepared_stmt, 1, unitId);
				return tenants_prepared_stmt;
}
struct sqlite3_stmt *get_receipts_by_tenant(unsigned int tenantId)
{
				struct sqlite3_stmt *receipts_by_tenant_stmt = NULL;
				sqlite3_prepare_v2(db,SELECT_RECEIPTS_BY_TENANT_QUERY,-1,&receipts_by_tenant_stmt,NULL);
				sqlite3_bind_int64(receipts_by_tenant_stmt,1,tenantId);
				return receipts_by_tenant_stmt;
}

struct sqlite3_stmt *get_receipts(struct sqlite3_stmt **receipts_prepared_stmt, struct receipt *outReceipt)
{
				if(*receipts_prepared_stmt == NULL) {
								sqlite3_prepare_v2(db,SELECT_RECEIPTS_QUERY,-1,receipts_prepared_stmt,NULL);
				}
				struct sqlite3_stmt *pReceipts_prepared_stmt = *receipts_prepared_stmt;
				if(sqlite3_step(pReceipts_prepared_stmt) == SQLITE_ROW) {
								outReceipt->id = sqlite3_column_int(pReceipts_prepared_stmt,0);
								outReceipt->name = sqlite3_column_text(pReceipts_prepared_stmt,1);
								outReceipt->tenant_id = sqlite3_column_int(pReceipts_prepared_stmt,2);
								outReceipt->cost = sqlite3_column_int(pReceipts_prepared_stmt,3);
								outReceipt->date = sqlite3_column_int(pReceipts_prepared_stmt,4);
								return pReceipts_prepared_stmt;
				}
				sqlite3_finalize(pReceipts_prepared_stmt);
				return NULL;
}

sqlite3_int64 insert_tenant(unsigned int unitId, const char *name)
{
				sqlite3_prepare_v2(db, INSERT_NEW_TENANT_STATEMENT, -1, &prepared_stmt, NULL);

				sqlite3_bind_int(prepared_stmt, 1, unitId);
				sqlite3_bind_text(prepared_stmt, 2, name, -1, SQLITE_STATIC);

				if (sqlite3_step(prepared_stmt) == SQLITE_DONE)
				{
								return sqlite3_last_insert_rowid(db);
				}
				else
				{
								return -1;
				}

				sqlite3_clear_bindings(prepared_stmt);
				sqlite3_finalize(prepared_stmt);
}
sqlite3_int64 insert_receipt(const char *name, unsigned int tenantId, unsigned int cost, unsigned long long date)
{
				sqlite3_prepare_v2(db, INSERT_NEW_RECEIPT_STATEMENT, -1, &prepared_stmt, NULL);

				sqlite3_bind_text(prepared_stmt, 1, name, -1, SQLITE_STATIC);
				sqlite3_bind_int(prepared_stmt, 2, tenantId);
				sqlite3_bind_int(prepared_stmt,3,cost);
				sqlite3_bind_int64(prepared_stmt,4,date);

				if (sqlite3_step(prepared_stmt) == SQLITE_DONE)
				{
								return sqlite3_last_insert_rowid(db);
				}
				else
				{
								return -1;
				}
				sqlite3_clear_bindings(prepared_stmt);
				sqlite3_finalize(prepared_stmt);
}

void close()
{
				sqlite3_close(db);
}

void init()
{
				sqlite3_open("billShare.db", &db);
				exec_sql(CREATE_TABLES_QUERY);
}
