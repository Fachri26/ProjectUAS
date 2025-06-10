#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

#include "include/penjual_mode.hpp"
#include "include/pembeli_mode.hpp"
#include "include/file_handler.hpp"
#include "include/exception_handler.hpp"

int main() {
    const std::string file_barang = "SistemBelanjaOnline/data/barang.txt";
    const std::string file_user = "SistemBelanjaOnline/data/user.txt";

    NodeBarang* root_barang = nullptr;
    UserTable user_table;

    // Muat data dari file
    try {
        root_barang = muat_data_barang(file_barang);
        muat_data_user(user_table, file_user);
    } catch (const std::exception& e) {
        std::cerr << "Gagal memuat data: " << e.what() << "\n";
        return 1;
    }

    // Menu utama
    std::vector<std::string> menu = {
        "1. Mode Penjual",
        "2. Mode Pembeli",
        "3. Keluar"
    };

    while (true) {
        std::cout << "\n\033[1;97m+==============================+\033[0m\n";  
        std::cout << "\033[1;97m|         MENU UTAMA           |\033[0m\n";  
        std::cout << "\033[1;97m+==============================+\033[0m\n"; 

        for (const auto& m : menu) {
            std::cout << "\033[1;97m| " << std::left << std::setw(29) << m << "|\033[0m\n";
        }

        std::cout << "\033[1;97m+==============================+\033[0m\n";
        std::cout << "Pilih opsi: ";
        std::cout << "\033[0m"; 

        std::string input;
        std::getline(std::cin, input);

        try {
            int pilihan = safe_stoi(input);
            if (pilihan == 1) {
                menu_penjual(root_barang, file_barang);
            } else if (pilihan == 2) {
                menu_awal_pembeli(user_table, root_barang);
            } else if (pilihan == 3) {
                // Simpan data sebelum keluar
                simpan_data_barang(root_barang, file_barang);
                simpan_data_user(user_table, file_user);
                std::cout << "Data disimpan. Terima kasih!\n";
                break;
            } else {
                std::cout << "\033[91mPilihan tidak valid.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "\033[91mInput error: " << e.what() << "\n";
        }
    }

    return 0;
}
