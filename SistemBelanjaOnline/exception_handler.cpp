// exception_handler.cpp
#include "include/exception_handler.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>

// Fungsi untuk konversi string ke int dengan penanganan exception
int safe_stoi(const std::string& input) {
    try {
        return std::stoi(input);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Input bukan angka yang valid.");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Angka terlalu besar atau kecil.");
    }
}

// Fungsi pembagian aman
int safe_divide(int numerator, int denominator) {
    if (denominator == 0) {
        throw std::runtime_error("Kesalahan: Pembagian dengan nol.");
    }
    return numerator / denominator;
}

// Fungsi untuk memastikan file berhasil dibuka
void cek_file_terbuka(bool status, const std::string& nama_file) {
    if (!status) {
        throw std::runtime_error("Gagal membuka file: " + nama_file);
    }
}