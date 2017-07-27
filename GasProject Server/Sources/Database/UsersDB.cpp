#include <iostream>
#include <fstream>

#include "UsersDB.hpp"
#include "Global.hpp"
#include "Server.hpp"

using namespace std;

UsersDB::UsersDB() : adr(Global::DatabaseName),
                     loaded(false) {
    adr = Global::DatabaseName;
    loaded = load();
    if (!loaded) Server::log << "Database reading error!" << endl;
}

bool UsersDB::load() {
    ifstream file;
    file.open(adr, ios::in);
    if (!file) return false;
    while (file) {
        string login, pass;
        file >> login;
        file >> pass;
        if (login != "") map[login] = pass;
    }
    file.close();
    return true;
}

bool UsersDB::Contain(const string &login) const {
    if (!loaded) return false;
    auto it = map.find(login);
    if (it == map.end()) return false;
    return true;
}

bool UsersDB::Check(const string &login, const string &pass) const {
    if (!loaded) return false;
    auto it = map.find(login);
    if (it == map.end()) return false;
    if (it->second != pass) return false;
    return true;
}

bool UsersDB::Add(const string &login, const string &pass) {
    if (!loaded) return false;
    if (login == "" || pass == "") return false;
    if (Contain(login)) return false;
    map[login] = pass;
    save();
    return true;
}

bool UsersDB::IsLoaded() const {
    return loaded;
}

bool UsersDB::save() {
    ofstream file;
    file.open(adr, ios::out);
    for (auto &pair : map) {
        file << pair.first << " " << pair.second << endl;
    }
    file.close();
    return false;
}