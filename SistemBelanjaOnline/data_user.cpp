#include "include/data_user.hpp"
#include <iostream>

// Fungsi untuk menambahkan user baru ke hash table
void insert_user(UserTable& table, const std::string& username, const std::string& password) {
    if (table.find(username) != table.end()) {
        throw std::runtime_error("Username sudah terdaftar.");
    }
    table[username] = password;
}

// Fungsi untuk verifikasi login user
bool verify_login(const UserTable& table, const std::string& username, const std::string& password) {
    auto it = table.find(username);
    if (it == table.end()) {
        throw std::runtime_error("Username tidak ditemukan.");
    }
    if (it->second != password) {
        throw std::runtime_error("Password salah.");
    }
    return true;
}
