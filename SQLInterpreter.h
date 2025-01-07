#ifndef SQL_INTERPRETER_H
#define SQL_INTERPRETER_H

#include <string>
#include <vector>
#include "FileManager.h"
#include "SchemaManager.h"
#include "RecordManager.h"

class SQLInterpreter {
private:
    FileManager fileManager;
    SchemaManager schemaManager;
    RecordManager recordManager;

public:
    void executeSQL(const std::string& sql, const std::string& dbName);
};

#endif // SQL_INTERPRETER_H
