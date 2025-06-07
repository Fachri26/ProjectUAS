// data_user.h
#ifndef DATA_USER_H
#define DATA_USER_H

#include <string>
#include <unordered_map>
#include <stdexcept>

// Struktur User
struct User {
    std::string username;
    std::string password;
};

// Tipe alias untuk hash table
using UserTable = std::unordered_map<std::string, std::string>;

// Fungsi
void insert_user(UserTable& table, const std::string& username, const std::string& password);
bool verify_login(const UserTable& table, const std::string& username, const std::string& password);

#endif // DATA_USER_H