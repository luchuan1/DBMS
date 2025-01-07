#include "RecordManager.h"
#include <iostream>
#include <vector>

void RecordManager::insertRecord(const std::string& tableName, const std::vector<std::string>& values) {
    std::cout << "Inserted record into table: " << tableName << std::endl;
}

void RecordManager::updateRecord(const std::string& tableName, const std::string& condition, const std::vector<std::string>& values) {
    std::cout << "Updated records in table: " << tableName << " where " << condition << std::endl;
}

void RecordManager::deleteRecord(const std::string& tableName, const std::string& condition) {
    std::cout << "Deleted records from table: " << tableName << " where " << condition << std::endl;
}
