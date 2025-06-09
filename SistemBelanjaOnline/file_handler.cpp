#include "include/file_handler.hpp"
#include "include/exception_handler.hpp"
#include <sstream>
#include <iostream>

// Fungsi bantu untuk parsing satu baris barang
Barang parse_barang(const std::string& line) {
    std::stringstream ss(line);
    std::string id, nama, stok, harga;
    std::getline(ss, id, ',');
    std::getline(ss, nama, ',');
    std::getline(ss, stok, ',');
    std::getline(ss, harga);

    if (id.empty() || nama.empty() || stok.empty() || harga.empty())
        throw std::runtime_error("Format data barang salah: " + line);

    Barang b;
    b.id = std::stoi(id);
    b.nama = nama;
    b.stok = std::stoi(stok);
    b.harga = std::stod(harga);
    return b;
}

// Simpan data barang (in-order traversal)
void simpan_data_barang(NodeBarang* root, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("Gagal membuka file: " + filename);
    inOrder(root, [&](Barang b) {
        out << b.id << "," << b.nama << "," << b.stok << "," << b.harga << "\n";
    });
    out.close();
}

// Fungsi untuk me-load data dari file
NodeBarang* muat_data_barang(const std::string& filename) {
    std::ifstream in(filename);
    cek_file_terbuka(in.is_open(), filename);
    NodeBarang* root = nullptr;
    std::string line;
    while (std::getline(in, line)) {
        try {
            Barang b = parse_barang(line);
            root = insertBarang(root, b);
        } catch (const std::exception& e) {
            std::cerr << "[Barang] Abaikan baris: " << line << ", karena: " << e.what() << "\n";
        }
    }
    in.close();
    return root;
}

// Simpan data user ke file
void simpan_data_user(const UserTable& table, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("Gagal membuka file: " + filename);
    for (const auto& [username, password] : table) {
        out << username << "," << password << "\n";
    }
    out.close();
}

// Muat data user dari file
void muat_data_user(UserTable& table, const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("Gagal membuka file: " + filename);
    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string username, password;
        std::getline(ss, username, ',');
        std::getline(ss, password);
        if (!username.empty() && !password.empty()) {
            table[username] = password;
        } else {
            std::cerr << "[User] Abaikan baris kosong atau rusak: " << line << "\n";
        }
    }
    in.close();
}

