#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <vector>

struct Field {
    std::string name;    // 字段名
    std::string type;    // 字段类型
    int length;          // 字段长度
};

struct TableSchema {
    std::string tableName;      // 表名
    std::vector<Field> fields;  // 字段信息
};

#endif // SCHEMA_H
