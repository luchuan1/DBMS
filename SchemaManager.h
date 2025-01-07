#ifndef SCHEMA_MANAGER_H
#define SCHEMA_MANAGER_H

#include <string>
#include <vector>

struct Field {
    std::string name;
    std::string type;
    int length;
};

struct TableSchema {
    std::string tableName;
    std::vector<Field> fields;
};

class SchemaManager {
public:
    void createTable(const std::string& dbName, const TableSchema& schema);
    void listTables(const std::string& dbName);
    void dropTable(const std::string& dbName, const std::string& tableName);
};

#endif // SCHEMA_MANAGER_H
