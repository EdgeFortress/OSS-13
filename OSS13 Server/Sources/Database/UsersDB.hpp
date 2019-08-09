#pragma once

#include <map>
#include <string>

class UsersDB {
private:
    std::string adr;
    bool loaded;
    std::map<std::string, std::string> map;
    bool load();
    bool save();

public:
    explicit UsersDB();
    UsersDB(const UsersDB &) = delete;
    UsersDB &operator=(const UsersDB &) = delete;
    ~UsersDB() = default;

    bool Check(const std::string &login, const std::string &pass) const;
    bool Contain(const std::string &login) const;
    bool Add(const std::string &login, const std::string &pass);
    bool IsLoaded() const;
};