#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>
#include <functional>

// Fungsi validasi input angka
bool is_number(const std::string& s);

// Fungsi pemisahan string sederhana
std::vector<std::string> split(const std::string& s, char delimiter);

// Lambda dan fungsi utilitas lainnya
extern std::function<bool(const std::string&)> lambda_validasi_username;
extern std::function<bool(int)> lambda_filter_stok_kritis;

#endif 