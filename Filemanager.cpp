#include "FileManager.h"
#include <iostream>
#include <fstream>

void FileManager::createDatabase(const std::string& dbName) {
    std::ofstream file(dbName, std::ios::binary);
    if (file.is_open()) {
        std::cout << "Database created: " << dbName << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to create database: " << dbName << std::endl;
    }
}

bool FileManager::databaseExists(const std::string& dbName) {
    std::ifstream file(dbName, std::ios::binary);
    return file.is_open();
}
