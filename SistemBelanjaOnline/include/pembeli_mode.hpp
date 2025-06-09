#ifndef PEMBELI_MODE_H
#define PEMBELI_MODE_H

#include <unordered_map>
#include <vector>
#include <string>
#include "data_barang.hpp"
#include "data_user.hpp"

// Struktur item keranjang (barang + qty)
struct KeranjangItem {
    Barang barang;
    int jumlah;
};

// Fungsi utama mode pembeli
void menu_pembeli(UserTable& user_table, NodeBarang* root_barang);
void menu_awal_pembeli(UserTable& user_table, NodeBarang* root_barang);

#endif 
