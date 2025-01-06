#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "Schema.h"

class FileManager {
public:
    void createDatabase(const std::string& dbName); // 创建数据库文件
    void writeTableSchema(const std::string& dbName, const TableSchema& schema); // 写入表模式
    TableSchema readTableSchema(const std::string& dbName, const std::string& tableName); // 读取表模式
};

#endif // FILE_MANAGER_H
