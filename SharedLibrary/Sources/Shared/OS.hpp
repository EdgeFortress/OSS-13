#pragma once

#include <string>
#include <list>
#include <filesystem>

// Compare string with wildcard
bool WildCompare(const std::string &string, const std::string &wild);

struct FileInfo {
    std::string name;
    std::string path;
    std::string extension;
};

// Get FileInfo by file path
FileInfo ParseFilePath(const std::string &filePath);

// Find the files by mask in the directory and subdirectories
// Return list of funded files
std::list<std::filesystem::path> FindFilesRecursive(const std::filesystem::path &root, const std::string &wildcard);