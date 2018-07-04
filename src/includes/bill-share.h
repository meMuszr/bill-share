#include <sqlite3.h>
#include <time.h>
struct unit
{
	unsigned char id;
	const unsigned char *name;
};
struct tenant
{
	unsigned char id;
	unsigned char unit_id;
	const unsigned char *name;
};
struct reciept
{
				unsigned char id;
				unsigned char tenant_id;
				const unsigned char *name;
				time_t time;
				unsigned int cost;
};
sqlite3_int64 insert_unit(const char *name);
sqlite3_int64 insert_tenant(unsigned int unitId, const char *name);
sqlite3_int64 insert_receipt(const char *name, unsigned int tenantId, unsigned int cost, unsigned long long date);
struct sqlite3_stmt *get_units(struct sqlite3_stmt **units_prepared_stmt, struct unit *outUnit);
struct sqlite3_stmt *get_tenants(struct sqlite3_stmt **tenants_prepared_stmt, struct tenant *outTenant);
struct sqlite3_stmt *get_tenants_by_unit(unsigned int unitId);
void close();
void init();
