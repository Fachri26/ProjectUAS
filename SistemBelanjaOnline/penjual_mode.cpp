// penjual_mode.cpp
#include "include/penjual_mode.hpp"
#include "include/file_handler.hpp"
#include "include/util_sort_search.hpp"
#include "include/exception_handler.hpp"
#include "include/helper.hpp"

#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

void tampilkan_semua(NodeBarang* root) {
    std::vector<Barang> daftar;
    inOrder(root, [&](Barang b) {
        daftar.push_back(b);
    });
    for (const auto& b : daftar) {
        std::cout << "ID: " << b.id << ", Nama: " << b.nama << ", Stok: " << b.stok << ", Harga: " << b.harga << '\n';
    }
}

void tampilkan_sort(NodeBarang* root) {
    std::vector<Barang> daftar;
    inOrder(root, [&](Barang b) {
        daftar.push_back(b);
    });

    std::cout << "\nUrut berdasarkan:\n1. Nama (A-Z)\n2. Stok (terbesar)\n> ";
    std::string pilihan;
    std::getline(std::cin, pilihan);

    if (pilihan == "1") sort_barang_by_nama(daftar);
    else if (pilihan == "2") sort_barang_by_stok(daftar);

    for (const auto& b : daftar) {
        std::cout << "ID: " << b.id << ", Nama: " << b.nama << ", Stok: " << b.stok << ", Harga: " << b.harga << '\n';
    }
}

void tambah_barang(NodeBarang*& root) {
    Barang b;
    std::string input;

    std::cout << "ID: ";
    std::getline(std::cin, input);
    b.id = safe_stoi(input);

    std::cout << "Nama: ";
    std::getline(std::cin, b.nama);

    std::cout << "Stok: ";
    std::getline(std::cin, input);
    b.stok = safe_stoi(input);

    std::cout << "Harga: ";
    std::getline(std::cin, input);
    b.harga = std::stod(input);

    if (cariBarang(root, b.id)) {
        std::cout << "ID sudah ada. Gagal tambah.\n";
        return;
    }

    root = insertBarang(root, b);
    std::cout << "Barang berhasil ditambahkan.\n";
}

void edit_barang(NodeBarang* root) {
    std::string input;
    std::cout << "Masukkan ID barang yang ingin diedit: ";
    std::getline(std::cin, input);
    int id = safe_stoi(input);

    NodeBarang* node = cariBarang(root, id);
    if (!node) {
        std::cout << "Barang tidak ditemukan.\n";
        return;
    }

    std::cout << "Edit nama (" << node->data.nama << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) node->data.nama = input;

    std::cout << "Edit stok (" << node->data.stok << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) node->data.stok = safe_stoi(input);

    std::cout << "Edit harga (" << node->data.harga << "): ";
    std::getline(std::cin, input);
    if (!input.empty()) node->data.harga = std::stod(input);

    std::cout << "Barang berhasil diedit.\n";
}

void hapus_barang(NodeBarang*& root) {
    std::string input;
    std::cout << "Masukkan ID barang yang ingin dihapus: ";
    std::getline(std::cin, input);
    int id = safe_stoi(input);

    if (!cariBarang(root, id)) {
        std::cout << "Barang tidak ditemukan.\n";
        return;
    }

    root = hapusBarang(root, id);
    std::cout << "Barang berhasil dihapus.\n";
}

void menu_penjual(NodeBarang*& root, const std::string& file_barang) {
    std::string pilihan;
    while (true) {
        std::cout << "\n===== Menu Penjual =====\n";
        std::cout << "1. Tampilkan semua barang\n";
        std::cout << "2. Tambah barang\n";
        std::cout << "3. Edit barang\n";
        std::cout << "4. Hapus barang\n";
        std::cout << "5. Urutkan & tampilkan barang\n";
        std::cout << "6. Simpan dan keluar\n> ";
        std::getline(std::cin, pilihan);

        try {
            if (pilihan == "1") tampilkan_semua(root);
            else if (pilihan == "2") tambah_barang(root);
            else if (pilihan == "3") edit_barang(root);
            else if (pilihan == "4") hapus_barang(root);
            else if (pilihan == "5") tampilkan_sort(root);
            else if (pilihan == "6") {
                simpan_data_barang(root, file_barang);
                std::cout << "Data berhasil disimpan.\n";
                break;
            } else {
                std::cout << "Pilihan tidak valid.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Terjadi kesalahan: " << e.what() << '\n';
        }
    }
}
