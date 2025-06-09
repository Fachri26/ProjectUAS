// util_sort_search.cpp
#include "include/util_sort_search.hpp"
#include "include/helper.hpp"
#include <cctype>
#include <algorithm> // sort, find_if, count_if

// Urutkan berdasarkan nama barang (ascending)
void sort_barang_by_nama(std::vector<Barang>& daftar) {
    std::sort(daftar.begin(), daftar.end(), [](const Barang& a, const Barang& b) {
        std::string nama_a = a.nama;
        std::string nama_b = b.nama;

        std::transform(nama_a.begin(), nama_a.end(), nama_a.begin(), ::tolower);
        std::transform(nama_b.begin(), nama_b.end(), nama_b.begin(), ::tolower);

        return nama_a < nama_b;
    });
}

// Urutkan berdasarkan stok barang (descending)
void sort_barang_by_stok(std::vector<Barang>& daftar) {
    std::sort(daftar.begin(), daftar.end(), [](const Barang& a, const Barang& b) {
        return a.stok > b.stok;
    });
}

// Cari barang berdasarkan nama (case-sensitive sederhana)
const Barang* cari_barang_by_nama(const std::vector<Barang>& daftar, const std::string& nama_dicari) {
    auto it = std::find_if(daftar.begin(), daftar.end(), [&](const Barang& b) {
        return b.nama == nama_dicari;
    });
    if (it != daftar.end()) return &(*it);
    return nullptr;
}

// Hitung barang yang stok-nya di bawah batas kritis
int hitung_barang_stok_rendah(const std::vector<Barang>& daftar) {
    return std::count_if(daftar.begin(), daftar.end(), [](const Barang& b) {
        return lambda_filter_stok_kritis(b.stok);
    });
}
