#include <iostream>
#include "FileManager.h"
#include "TableManager.h"

int main() {
    FileManager fileManager;
    TableManager tableManager;

    // 文件管理模块测试
    std::string dbName = "test_db.bin";
    std::cout << "Testing FileManager Module..." << std::endl;

    if (fileManager.createDatabase(dbName)) {
        std::cout << "Database file created successfully: " << dbName << std::endl;
    } else {
        std::cerr << "Failed to create database file: " << dbName << std::endl;
    }

    TableSchema schema;
    schema.tableName = "Employees";
    schema.fields = {
        {"ID", "INT", 4},
        {"Name", "VARCHAR", 50},
        {"Salary", "FLOAT", 8}
    };

    if (fileManager.writeTableSchema(dbName, schema)) {
        std::cout << "Table schema written successfully for table: " << schema.tableName << std::endl;
    } else {
        std::cerr << "Failed to write table schema." << std::endl;
    }

    TableSchema readSchema = fileManager.readTableSchema(dbName, "Employees");
    std::cout << "Read Table Schema for table: " << readSchema.tableName << std::endl;
    for (const auto& field : readSchema.fields) {
        std::cout << "Field: " << field.name << ", Type: " << field.type << ", Length: " << field.length << std::endl;
    }

    // 表构造模式管理模块测试
    std::cout << "\nTesting TableManager Module..." << std::endl;

    // 添加字段
    Field newField{"Department", "VARCHAR", 30};
    if (tableManager.addField(dbName, "Employees", newField)) {
        std::cout << "Added new field: " << newField.name << std::endl;
    }

    // 删除字段
    if (tableManager.deleteField(dbName, "Employees", "Salary")) {
        std::cout << "Deleted field: Salary" << std::endl;
    }

    // 修改字段名
    if (tableManager.renameField(dbName, "Employees", "Department", "Dept")) {
        std::cout << "Renamed field 'Department' to 'Dept'" << std::endl;
    }

    // 修改字段类型或长度
    if (tableManager.modifyField(dbName, "Employees", "Dept", "CHAR", 20)) {
        std::cout << "Modified field 'Dept' to type 'CHAR' with length 20" << std::endl;
    }

    return 0;
}
