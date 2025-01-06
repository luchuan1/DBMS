#include "FileManager.h"
#include <iostream>
#include <fstream>

void FileManager::createDatabase(const std::string& dbName) {
    std::ofstream file(dbName, std::ios::binary);
    if (file.is_open()) {
        std::cout << "Database file created: " << dbName << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to create database file: " << dbName << std::endl;
    }
}

void FileManager::writeTableSchema(const std::string& dbName, const TableSchema& schema) {
    std::ofstream file(dbName, std::ios::binary | std::ios::app);
    if (file.is_open()) {
        size_t tableNameLength = schema.tableName.size();
        file.write(reinterpret_cast<char*>(&tableNameLength), sizeof(tableNameLength));
        file.write(schema.tableName.c_str(), tableNameLength);

        size_t fieldCount = schema.fields.size();
        file.write(reinterpret_cast<char*>(&fieldCount), sizeof(fieldCount));
        for (const auto& field : schema.fields) {
            size_t fieldNameLength = field.name.size();
            file.write(reinterpret_cast<char*>(&fieldNameLength), sizeof(fieldNameLength));
            file.write(field.name.c_str(), fieldNameLength);

            size_t fieldTypeLength = field.type.size();
            file.write(reinterpret_cast<char*>(&fieldTypeLength), sizeof(fieldTypeLength));
            file.write(field.type.c_str(), fieldTypeLength);

            file.write(reinterpret_cast<char*>(&field.length), sizeof(field.length));
        }
        file.close();
        std::cout << "Table schema written to database file: " << schema.tableName << std::endl;
    } else {
        std::cerr << "Failed to write to database file: " << dbName << std::endl;
    }
}

TableSchema FileManager::readTableSchema(const std::string& dbName, const std::string& tableName) {
    std::ifstream file(dbName, std::ios::binary);
    TableSchema schema;

    if (file.is_open()) {
        while (!file.eof()) {
            size_t tableNameLength;
            file.read(reinterpret_cast<char*>(&tableNameLength), sizeof(tableNameLength));

            std::string currentTableName(tableNameLength, '\0');
            file.read(&currentTableName[0], tableNameLength);

            size_t fieldCount;
            file.read(reinterpret_cast<char*>(&fieldCount), sizeof(fieldCount));

            std::vector<Field> fields;
            for (size_t i = 0; i < fieldCount; ++i) {
                size_t fieldNameLength;
                file.read(reinterpret_cast<char*>(&fieldNameLength), sizeof(fieldNameLength));

                std::string fieldName(fieldNameLength, '\0');
                file.read(&fieldName[0], fieldNameLength);

                size_t fieldTypeLength;
                file.read(reinterpret_cast<char*>(&fieldTypeLength), sizeof(fieldTypeLength));

                std::string fieldType(fieldTypeLength, '\0');
                file.read(&fieldType[0], fieldTypeLength);

                int fieldLength;
                file.read(reinterpret_cast<char*>(&fieldLength), sizeof(fieldLength));

                fields.push_back({fieldName, fieldType, fieldLength});
            }

            if (currentTableName == tableName) {
                schema.tableName = currentTableName;
                schema.fields = fields;
                break;
            }
        }
        file.close();
    } else {
        std::cerr << "Failed to read from database file: " << dbName << std::endl;
    }

    return schema;
}
