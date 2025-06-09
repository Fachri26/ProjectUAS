#include "include/helper.hpp"
#include <sstream>
#include <cctype>

// Fungsi untuk mengecek apakah string adalah angka
bool is_number(const std::string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return !s.empty();
}

// Fungsi untuk memisahkan string berdasarkan delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

// Lambda untuk validasi username (alfanumerik dan minimal 3 karakter)
std::function<bool(const std::string&)> lambda_validasi_username = [](const std::string& username) {
    if (username.length() < 3) return false;
    for (char c : username) {
        if (!isalnum(c)) return false;
    }
    return true;
};

// Lambda untuk filter stok barang rendah (< 10)
std::function<bool(int)> lambda_filter_stok_kritis = [](int stok) {
    return stok < 10;
};
