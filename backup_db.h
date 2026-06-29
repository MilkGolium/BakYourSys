#ifndef BACKUP_DB_H
#define BACKUP_DB_H

#include <sqlite3.h>

/*
 * Open a connection to the SQLite database at dbPath.
 * Enforces PRAGMA foreign_keys = ON immediately after opening.
 * Returns 1 on success, 0 on failure.
 * The caller must free *db via dbCloseConnection() when done.
 */
int
dbInitConnection(const char* dbPath, sqlite3** db);

/*
 * Close a database connection previously opened by dbInitConnection().
 * Returns 1 on success, 0 on failure.
 */
int
dbCloseConnection(sqlite3* db);

/*
 * Open app.db, print the number of rows in app_configs to stdout,
 * then close the connection.
 * Returns 1 on success (query executed), 0 on failure.
 * This is a smoke-test function for verifying the build.
 */
int
dbTestConnection(const char* dbPath);

#endif /* BACKUP_DB_H */
