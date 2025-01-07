#include "SchemaManager.h"
#include <iostream>
#include <fstream>

void SchemaManager::createTable(const std::string& dbName, const TableSchema& schema) {
    std::ofstream file(dbName, std::ios::binary | std::ios::app);
    if (file.is_open()) {
        size_t tableNameLength = schema.tableName.size();
        file.write(reinterpret_cast<const char*>(&tableNameLength), sizeof(tableNameLength));
        file.write(schema.tableName.c_str(), tableNameLength);

        size_t fieldCount = schema.fields.size();
        file.write(reinterpret_cast<const char*>(&fieldCount), sizeof(fieldCount));

        for (const auto& field : schema.fields) {
            size_t fieldNameLength = field.name.size();
            file.write(reinterpret_cast<const char*>(&fieldNameLength), sizeof(fieldNameLength));
            file.write(field.name.c_str(), fieldNameLength);

            size_t fieldTypeLength = field.type.size();
            file.write(reinterpret_cast<const char*>(&fieldTypeLength), sizeof(fieldTypeLength));
            file.write(field.type.c_str(), fieldTypeLength);

            file.write(reinterpret_cast<const char*>(&field.length), sizeof(field.length));
        }

        std::cout << "Table created: " << schema.tableName << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to create table in database: " << dbName << std::endl;
    }
}

void SchemaManager::listTables(const std::string& dbName) {
    std::ifstream file(dbName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Database not found: " << dbName << std::endl;
        return;
    }

    std::cout << "Tables in database:" << std::endl;
    while (!file.eof()) {
        size_t tableNameLength;
        file.read(reinterpret_cast<char*>(&tableNameLength), sizeof(tableNameLength));
        if (file.eof()) break;

        std::string tableName(tableNameLength, '\0');
        file.read(&tableName[0], tableNameLength);

        size_t fieldCount;
        file.read(reinterpret_cast<char*>(&fieldCount), sizeof(fieldCount));

        file.seekg(fieldCount * (sizeof(size_t) * 2 + sizeof(int)), std::ios::cur);
        std::cout << "- " << tableName << std::endl;
    }

    file.close();
}

void SchemaManager::dropTable(const std::string& dbName, const std::string& tableName) {
    // Basic implementation: Rebuild the file without the target table
    std::ifstream file(dbName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Database not found: " << dbName << std::endl;
        return;
    }

    std::ofstream tempFile("temp.db", std::ios::binary);
    while (!file.eof()) {
        size_t tableNameLength;
        file.read(reinterpret_cast<char*>(&tableNameLength), sizeof(tableNameLength));
        if (file.eof()) break;

        std::string currentTableName(tableNameLength, '\0');
        file.read(&currentTableName[0], tableNameLength);

        size_t fieldCount;
        file.read(reinterpret_cast<char*>(&fieldCount), sizeof(fieldCount));

        std::vector<char> tableData(fieldCount * (sizeof(size_t) * 2 + sizeof(int)));
        file.read(tableData.data(), tableData.size());

        if (currentTableName != tableName) {
            tempFile.write(reinterpret_cast<const char*>(&tableNameLength), sizeof(tableNameLength));
            tempFile.write(currentTableName.c_str(), tableNameLength);
            tempFile.write(reinterpret_cast<const char*>(&fieldCount), sizeof(fieldCount));
            tempFile.write(tableData.data(), tableData.size());
        }
    }

    file.close();
    tempFile.close();

    std::remove(dbName.c_str());
    std::rename("temp.db", dbName.c_str());

    std::cout << "Table dropped: " << tableName << std::endl;
}
