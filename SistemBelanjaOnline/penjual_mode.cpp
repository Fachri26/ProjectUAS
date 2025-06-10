#include "include/penjual_mode.hpp"
#include "include/file_handler.hpp"
#include "include/util_sort_search.hpp"
#include "include/exception_handler.hpp"
#include "include/helper.hpp"

#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <iomanip>

// Menampilkan seluruh data barang
void tampilkan_semua(NodeBarang* root) {
    std::vector<Barang> daftar;
    inOrder(root, [&](Barang b) {
        daftar.push_back(b);
    });
    std::cout << "\033[1;97m== Daftar Barang ==\033[0m\n";
    std::cout << "+------+----------------------+--------+--------+\n";
    std::cout << "| \033[1;97mID\033[0m   | \033[1;97mNama Barang\033[0m          | \033[1;97mStok\033[0m    | \033[1;97mHarga\033[0m |\n";
    std::cout << "+------+----------------------+--------+--------+\n";

    for (const auto& b : daftar) {
        std::cout << "| "
                  << std::right << std::setw(4) << b.id << " | "
                  << std::left  << std::setw(20) << b.nama << " | "
                  << std::right << std::setw(6) << b.stok << " | "
                  << std::right << std::setw(6) << b.harga << " |\n";
    }

    std::cout << "+------+----------------------+--------+--------+\n";
}

// Memberikan pilihan untuk urut berdasarkan nama barang atau stok
void tampilkan_sort(NodeBarang* root) {
    std::vector<Barang> daftar;
    inOrder(root, [&](Barang b) {
        daftar.push_back(b);
    });

    std::cout << "\n\033[93mUrut berdasarkan:\033[0m\n1. Nama (A-Z)\n2. Stok (terbesar)\nPilihan:\033[0m ";
    std::string pilihan;
    std::getline(std::cin, pilihan);

    if (pilihan == "1") sort_barang_by_nama(daftar);
    else if (pilihan == "2") sort_barang_by_stok(daftar);
    else {
        std::cout << "\033[31mPilihan tidak valid. Kembali ke menu.\033[0m\n";
        return;
    }

    std::cout << "\n\033[1;97m== Barang Terurut ==\033[0m\n";
    std::cout << "+------+----------------------+--------+--------+\n";
    std::cout << "| \033[1;97mID\033[0m   | \033[1;97mNama Barang\033[0m          | \033[1;97mStok\033[0m   | \033[1;97mHarga\033[0m  |\n";
    std::cout << "+------+----------------------+--------+--------+\n";

    for (const auto& b : daftar) {
        std::cout << "| "
                  << std::right << std::setw(4) << b.id << " | "
                  << std::left  << std::setw(20) << b.nama << " | "
                  << std::right << std::setw(6) << b.stok << " | "
                  << std::right << std::setw(6) << b.harga << " |\n";
    }

    std::cout << "+------+----------------------+--------+--------+\n";
}

// Menampilkan stok yang dibawah 10
void tampilkan_stok_rendah(NodeBarang* root) {
    std::vector<Barang> daftar;

    inOrder(root, [&](Barang b) {
        if (b.stok < 10) {  
            daftar.push_back(b);
        }
    });

    if (daftar.empty()) {
        std::cout << "\033[91mTidak ada barang dengan stok rendah.\033[0m\n";
        return;
    }

    std::cout << "\n\033[1;97m== Barang dengan Stok Rendah ==\033[0m\n";
    std::cout << "+------+----------------------+--------+\n";
    std::cout << "| \033[1;97mID\033[0m   | \033[1;97mNama Barang\033[0m          | \033[1;97mStok\033[0m   |\n";
    std::cout << "+------+----------------------+--------+\n";

    for (const auto& b : daftar) {
        std::cout << "| "
                  << std::right << std::setw(4) << b.id << " | "
                  << std::left  << std::setw(20) << b.nama << " | "
                  << std::right << std::setw(6) << b.stok << " |\n";
    }

    std::cout << "+------+----------------------+--------+\n";
    std::cout << "\033[93mTotal barang dengan stok rendah:\033[0m " << daftar.size() << '\n';
}

// Menambahkan barang
void tambah_barang(NodeBarang*& root) {
    Barang b;
    std::string input;

    std::cout << "\n\033[93m== Tambah Barang Baru ==\033[0m\n";
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
        std::cout << "\033[91mID sudah ada. Gagal tambah.\033[0m\n";
        return;
    }

    root = insertBarang(root, b);
    std::cout << "\033[92mBarang berhasil ditambahkan.\033[0m\n";
}

// Mengedit barang
void edit_barang(NodeBarang* root) {
    std::string input;
    std::cout << "\n\033[93m== Edit Barang ==\033[0m\n";
    if (!root) {
        std::cout << "\033[91mTidak ada barang untuk diedit.\033[0m\n";
        return;
    }
    std::cout << "Masukkan ID barang yang ingin diedit: ";
    std::getline(std::cin, input);
    int id = safe_stoi(input);

    NodeBarang* node = cariBarang(root, id);
    if (!node) {
        std::cout << "\033[91mBarang dengan ID " << id << " tidak ditemukan.\033[0m\n";
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

    std::cout << "\033[92mBarang berhasil diedit.\033[0m\n";
}

// Menghapus barang
void hapus_barang(NodeBarang*& root) {
    std::string input;
    std::cout << "\n\033[93m== Hapus Barang ==\033[0m\n";
    if (!root) {
        std::cout << "\033[91mTidak ada barang untuk dihapus.\033[0m\n";
        return;
    }
    std::cout << "Masukkan ID barang yang ingin dihapus: ";
    std::getline(std::cin, input);
    int id = safe_stoi(input);

    if (!cariBarang(root, id)) {
        std::cout << "\033[91mBarang dengan ID " << id << " tidak ditemukan.\033[0m\n";
        return;
    }

    root = hapusBarang(root, id);
    std::cout << "\033[92mBarang berhasil dihapus.\033[0m\n";
}

// Menu utama mode penjual
void menu_penjual(NodeBarang*& root, const std::string& file_barang) {
    std::string pilihan;
    while (true) {
        std::cout << "\033[1;36m\n+====================================+\n";
        std::cout << "\033[1;36m|\033[1;37m         == MENU PENJUAL ==         \033[1;36m|\n";
        std::cout << "\033[1;36m+====================================+\033[0m\n";

        std::vector<std::string> menu = {
            "1. Tampilkan semua barang",
            "2. Tambah barang",
            "3. Edit barang",
            "4. Hapus barang",
            "5. Urutkan & tampilkan barang",
            "6. Barang stok rendah",
            "7. Simpan dan keluar"
        };
        for (const auto& m : menu) {
            std::cout << "\033[1;36m|\033[0m " 
                      << std::left << std::setw(35) << m 
                      << "\033[1;36m|\033[0m\n";
        }
        std::cout << "\033[1;36m+====================================+\033[0m\n";

        std::cout << "\033[1;33m> Pilihan: \033[0m";
        std::getline(std::cin, pilihan);

        try {
            if (pilihan == "1") tampilkan_semua(root);
            else if (pilihan == "2") tambah_barang(root);
            else if (pilihan == "3") edit_barang(root);
            else if (pilihan == "4") hapus_barang(root);
            else if (pilihan == "5") tampilkan_sort(root);
            else if (pilihan == "6") tampilkan_stok_rendah(root);
            else if (pilihan == "7") {
                simpan_data_barang(root, file_barang);
                std::cout << "\033[92mData berhasil disimpan.\033[0m\n";
                break;
            } else {
                std::cout << "\033[91mPilihan tidak valid.\033[0m\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "\033[91mTerjadi kesalahan: " << e.what() << "\033[0m\n";
        }
    }
}
