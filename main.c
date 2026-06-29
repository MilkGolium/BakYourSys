#include "backup_db.h"

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    const char* dbPath = "app.db";

    printf("[BackupEngine] Starting database connection test...\n");
    printf("[BackupEngine] Target database: %s\n", dbPath);

    int result = dbTestConnection(dbPath);

    if (result) {
        printf("[BackupEngine] Test PASSED.\n");
    } else {
        fprintf(stderr, "[BackupEngine] Test FAILED.\n");
        return 1;
    }

    return 0;
}
