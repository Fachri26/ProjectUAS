// exception_handler.h
#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

#include <string>

// Fungsi untuk validasi input integer (dengan pengecekan exception)
int safe_stoi(const std::string& input);

// Fungsi untuk menangani pembagian dengan nol
int safe_divide(int numerator, int denominator);

// Fungsi untuk cek keberadaan file dan lemparkan exception jika tidak ada
void cek_file_terbuka(bool status, const std::string& nama_file);

#endif // EXCEPTION_HANDLER_H