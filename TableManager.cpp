#include "TableManager.h"
#include <iostream>

TableManager::TableManager() {}

// 添加字段
bool TableManager::addField(const std::string& dbName, const std::string& tableName, const Field& newField) {
    TableSchema schema = fileManager.readTableSchema(dbName, tableName);

    // 检查字段是否已存在
    for (const auto& field : schema.fields) {
        if (field.name == newField.name) {
            std::cerr << "Field already exists: " << newField.name << std::endl;
            return false;
        }
    }

    schema.fields.push_back(newField);
    fileManager.writeTableSchema(dbName, schema);
    std::cout << "Field added successfully: " << newField.name << std::endl;
    return true;
}

// 删除字段
bool TableManager::deleteField(const std::string& dbName, const std::string& tableName, const std::string& fieldName) {
    TableSchema schema = fileManager.readTableSchema(dbName, tableName);

    auto it = std::remove_if(schema.fields.begin(), schema.fields.end(),
                             [&fieldName](const Field& field) { return field.name == fieldName; });

    if (it == schema.fields.end()) {
        std::cerr << "Field not found: " << fieldName << std::endl;
        return false;
    }

    schema.fields.erase(it, schema.fields.end());
    fileManager.writeTableSchema(dbName, schema);
    std::cout << "Field deleted successfully: " << fieldName << std::endl;
    return true;
}

// 修改字段名
bool TableManager::renameField(const std::string& dbName, const std::string& tableName, const std::string& oldName, const std::string& newName) {
    TableSchema schema = fileManager.readTableSchema(dbName, tableName);

    for (auto& field : schema.fields) {
        if (field.name == oldName) {
            field.name = newName;
            fileManager.writeTableSchema(dbName, schema);
            std::cout << "Field renamed successfully: " << oldName << " to " << newName << std::endl;
            return true;
        }
    }

    std::cerr << "Field not found: " << oldName << std::endl;
    return false;
}

// 修改字段类型或长度
bool TableManager::modifyField(const std::string& dbName, const std::string& tableName, const std::string& fieldName, const std::string& newType, int newLength) {
    TableSchema schema = fileManager.readTableSchema(dbName, tableName);

    for (auto& field : schema.fields) {
        if (field.name == fieldName) {
            field.type = newType;
            field.length = newLength;
            fileManager.writeTableSchema(dbName, schema);
            std::cout << "Field modified successfully: " << fieldName << std::endl;
            return true;
        }
    }

    std::cerr << "Field not found: " << fieldName << std::endl;
    return false;
}
