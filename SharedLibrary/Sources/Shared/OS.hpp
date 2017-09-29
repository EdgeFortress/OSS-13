#pragma once

#include <string>
#include <list>

// Compare string with wildcard
bool WildCompare(const std::wstring &string, const std::wstring &wild);

struct FileInfo {
    std::wstring name;
    std::wstring path;
    std::wstring extension;
};

// Get FileInfo by file path
FileInfo ParseFilePath(const std::wstring &filePath);

// Find the files by mask in the directory and subdirectories
// Return list of funded files
std::list<std::wstring> FindFilesRecursive(std::wstring path, const std::wstring name);