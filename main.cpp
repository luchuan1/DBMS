#include "FileManager.h"
#include <iostream>

int main() {
    FileManager fileManager;

    // Step 1: 创建数据库文件
    std::string dbName = "test_db.bin";
    fileManager.createDatabase(dbName);

    // Step 2: 写入表模式
    TableSchema schema;
    schema.tableName = "Employees";
    schema.fields = {
        {"ID", "INT", 4},
        {"Name", "VARCHAR", 50},
        {"Salary", "FLOAT", 8}
    };
    fileManager.writeTableSchema(dbName, schema);

    // Step 3: 读取表模式
    TableSchema readSchema = fileManager.readTableSchema(dbName, "Employees");

    // 输出读取的表模式
    std::cout << "Table Name: " << readSchema.tableName << std::endl;
    for (const auto& field : readSchema.fields) {
        std::cout << "Field: " << field.name << ", Type: " << field.type << ", Length: " << field.length << std::endl;
    }

    return 0;
}
