#include "OS.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool WildCompare(const std::string &string, const std::string &wild) {
	auto ch = string.begin();
	auto wild_ch = wild.begin();

	while (ch != string.end() && *wild_ch != '*') {
		if (*wild_ch != *ch && *wild_ch != '?')
			return false;
		ch++;
		wild_ch++;
	}

	auto not_processed = string.begin();
	auto wild_pos_after_asterisk = wild.begin();

	while (ch != string.end()) {
		if (wild_ch == wild.end())
			return false;
		if (*wild_ch == '*') { // Asterisk
			wild_ch++;
			if (wild_ch == wild.end()) {
				return true;
			}
			wild_pos_after_asterisk = wild_ch;
			not_processed = ch + 1; // skip 1 character as *
		} else if (*wild_ch == *ch || *wild_ch == '?') { // Symbols coincided
			wild_ch++;
			ch++;
		} else { // Symbols not coincided, so treat them as *
			wild_ch = wild_pos_after_asterisk;
			ch = ++not_processed;
		}
	} // String ended

	while (wild_ch != wild.end() && *wild_ch == '*') {
		wild_ch++;
	}

	return (wild_ch == wild.end());
}

FileInfo ParseFilePath(const std::string &filePath) {
	auto dot = filePath.end();
	auto slash = filePath.end();

	auto iter = filePath.end();
	while (iter != filePath.begin()) {
		iter--;
		if (*iter == L'.') {
			dot = iter;
		} else if (*iter == L'/') {
			slash = iter;
			break;
		}
	}

	// Form result
	FileInfo result;
	if (slash != filePath.end()) {
		result.path = std::string(filePath.begin(), slash);
		result.name = std::string(slash + 1, dot);
	} else {
		result.name = std::string(filePath.begin(), dot);
	}
	result.extension = std::string(dot, filePath.end());

	return result;
}

std::list<std::filesystem::path> FindFilesRecursive(const std::filesystem::path &root, const std::string &wildcard) {
	std::list<std::filesystem::path> result;

	for (fs::recursive_directory_iterator i(root), end; i != end; i++)
		if (!is_directory(i->path())) {
			if (WildCompare(i->path().filename().string(), wildcard)) {
				result.push_back(i->path());
			}
		}

	return result;
}
