#ifndef RECORD_MANAGER_H
#define RECORD_MANAGER_H
#include <string>
#include <vector>

class RecordManager {
public:
    void insertRecord(const std::string& tableName, const std::vector<std::string>& values);
    void updateRecord(const std::string& tableName, const std::string& condition, const std::vector<std::string>& values);
    void deleteRecord(const std::string& tableName, const std::string& condition);
};

#endif // RECORD_MANAGER_H
