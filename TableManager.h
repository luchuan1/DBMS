#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include "Schema.h"
#include "FileManager.h"
#include <string>
#include <vector>

class TableManager {
private:
    FileManager fileManager;

public:
    TableManager();

    // 添加字段
    bool addField(const std::string& dbName, const std::string& tableName, const Field& newField);

    // 删除字段
    bool deleteField(const std::string& dbName, const std::string& tableName, const std::string& fieldName);

    // 修改字段名
    bool renameField(const std::string& dbName, const std::string& tableName, const std::string& oldName, const std::string& newName);

    // 修改字段类型或长度
    bool modifyField(const std::string& dbName, const std::string& tableName, const std::string& fieldName, const std::string& newType, int newLength);
};

#endif // TABLEMANAGER_H
