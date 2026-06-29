#include "backup_db.h"

#include <stdio.h>

/* ---- Internal helpers ---- */

static int
execPragmaForeignKeys(sqlite3* db)
{
    char* errMsg = NULL;
    int rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[BackupEngine] PRAGMA foreign_keys failed: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }
    return 1;
}

/* ---- Public API ---- */

int
dbInitConnection(const char* dbPath, sqlite3** db)
{
    if (dbPath == NULL || db == NULL) {
        fprintf(stderr, "[BackupEngine] dbInitConnection: invalid arguments\n");
        return 0;
    }

    int rc = sqlite3_open(dbPath, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[BackupEngine] Cannot open database '%s': %s\n",
                dbPath, sqlite3_errmsg(*db));
        sqlite3_close(*db);
        *db = NULL;
        return 0;
    }

    if (!execPragmaForeignKeys(*db)) {
        sqlite3_close(*db);
        *db = NULL;
        return 0;
    }

    return 1;
}

int
dbCloseConnection(sqlite3* db)
{
    if (db == NULL) {
        return 0;
    }
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[BackupEngine] Error closing database: %s\n",
                sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

int
dbTestConnection(const char* dbPath)
{
    sqlite3* db = NULL;

    if (!dbInitConnection(dbPath, &db)) {
        return 0;
    }

    const char* sql = "SELECT COUNT(*) FROM app_configs;";
    sqlite3_stmt* stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[BackupEngine] Query prepare failed: %s\n",
                sqlite3_errmsg(db));
        dbCloseConnection(db);
        return 0;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        printf("[BackupEngine] Number of config entries: %d\n", count);
    } else {
        fprintf(stderr, "[BackupEngine] Query step failed: %s\n",
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        dbCloseConnection(db);
        return 0;
    }

    sqlite3_finalize(stmt);

    if (!dbCloseConnection(db)) {
        return 0;
    }

    return 1;
}
