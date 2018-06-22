#include <sqlite3.h>
struct unit {
				unsigned char id;
				const unsigned char *name;
};
sqlite3_int64 insert_unit(const char *name);
struct sqlite3_stmt *get_units(struct sqlite3_stmt **units_prepared_stmt, struct unit *outUnit);
struct sqlite3_stmt *get_tenants(unsigned int unitId);
sqlite3_int64 insert_tenant(unsigned int unitId, const char *name);
void close();
void init();


