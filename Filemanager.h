#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    void createDatabase(const std::string& dbName);
    bool databaseExists(const std::string& dbName);
};

#endif // FILE_MANAGER_H
