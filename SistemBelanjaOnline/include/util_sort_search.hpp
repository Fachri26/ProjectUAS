// util_sort_search.h
#ifndef UTIL_SORT_SEARCH_H
#define UTIL_SORT_SEARCH_H

#include <vector>
#include <string>
#include "data_barang.hpp"

// Fungsi
void sort_barang_by_nama(std::vector<Barang>& daftar);
void sort_barang_by_stok(std::vector<Barang>& daftar);
const Barang* cari_barang_by_nama(const std::vector<Barang>& daftar, const std::string& nama_dicari);
int hitung_barang_stok_rendah(const std::vector<Barang>& daftar);

#endif // UTIL_SORT_SEARCH_H