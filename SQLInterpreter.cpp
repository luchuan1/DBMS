#include "SQLInterpreter.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void SQLInterpreter::executeSQL(const std::string& sql, const std::string& dbName) {
    std::istringstream stream(sql);
    std::string command;
    stream >> command;
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "CREATE") {
        std::string nextWord;
        stream >> nextWord;
        if (nextWord == "TABLE") {
            std::string tableName;
            stream >> tableName;

            TableSchema schema;
            schema.tableName = tableName;

            std::string fieldDef;
            while (std::getline(stream, fieldDef, ',')) {
                std::istringstream fieldStream(fieldDef);
                Field field;

                fieldStream >> field.name >> field.type >> field.length;
                schema.fields.push_back(field);
            }

            schemaManager.createTable(dbName, schema);
        } else if (nextWord == "DATABASE") {
            std::string databaseName;
            stream >> databaseName;
            fileManager.createDatabase(databaseName);
        }
    } else if (command == "DROP") {
        std::string nextWord;
        stream >> nextWord;
        if (nextWord == "TABLE") {
            std::string tableName;
            stream >> tableName;
            schemaManager.dropTable(dbName, tableName);
        }
    } else if (command == "INSERT") {
        std::string nextWord, tableName;
        stream >> nextWord >> tableName;
        std::string valuesClause;
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '(');
        std::getline(stream, valuesClause, ')');

        std::istringstream valuesStream(valuesClause);
        std::vector<std::string> values;
        std::string value;

        while (std::getline(valuesStream, value, ',')) {
            values.push_back(value);
        }

        recordManager.insertRecord(tableName, values);
    } else if (command == "DELETE") {
        std::string nextWord, tableName;
        stream >> nextWord >> tableName;

        std::string whereClause;
        std::getline(stream, whereClause);

        recordManager.deleteRecord(tableName, whereClause);
    } else if (command == "SELECT") {
        std::string fields, from, tableName, whereClause;
        stream >> fields >> from >> tableName;

        if (stream >> whereClause) {
            std::cout << "Querying with WHERE: " << whereClause << std::endl;
        } else {
            std::cout << "Querying all records from table: " << tableName << std::endl;
        }
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}
