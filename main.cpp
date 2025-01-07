#include <iostream>
#include <string>
#include "SQLInterpreter.h"

int main() {
    std::string dbName = "example.db";
    FileManager fileManager;
    SQLInterpreter sqlInterpreter;

    if (!fileManager.databaseExists(dbName)) {
        fileManager.createDatabase(dbName);
    }

    while (true) {
        std::cout << "SQL> ";
        std::string sql;
        std::getline(std::cin, sql);

        if (sql == "EXIT") {
            break;
        }

        sqlInterpreter.executeSQL(sql, dbName);
    }

    return 0;
}
