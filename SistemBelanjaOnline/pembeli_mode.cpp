#include "include/pembeli_mode.hpp"
#include "include/exception_handler.hpp"
#include "include/graph_jaringan.hpp"
#include "include/file_handler.hpp"
#include "include/helper.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

// Fungsi untuk login pembeli
bool login_pembeli(const UserTable& user_table, std::string& username) {
    std::string input_user, input_pass;
    std::cout << "=== Login Pembeli ===\n";
    std::cout << "Username: "; std::getline(std::cin, input_user);
    std::cout << "Password: "; std::getline(std::cin, input_pass);

    try {
        if (verify_login(user_table, input_user, input_pass)) {
            username = input_user;
            std::cout << "Login berhasil.\n";
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Login gagal: " << e.what() << "\n";
    }
    return false;
}

//Fungsi untuk register user
bool register_user(UserTable& user_table, const std::string& filename) {
    std::string username, password;
    std::cout << "=== Registrasi Pengguna Baru ===\n";
    std::cout << "Username baru: "; std::getline(std::cin, username);

    if (!lambda_validasi_username(username)) {
        std::cerr << "Username tidak valid. Harus minimal 3 karakter dan hanya huruf/angka.\n";
        return false;
    }

    std::cout << "Password: "; std::getline(std::cin, password);

    try {
        insert_user(user_table, username, password);
        simpan_data_user(user_table, filename); // Simpan ke file
        std::cout << "Registrasi berhasil. Silakan login.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Registrasi gagal: " << e.what() << "\n";
    }
    return false;
}

// Fungsi untuk memilih login atau register
void menu_awal_pembeli(UserTable& user_table, NodeBarang* root_barang) {
    const std::string user_data_file = "SistemBelanjaOnline/data/user.txt";

    while (true) {
        std::cout << "\033[1;36m\n+=============================================+\n";
        std::cout << "\033[1;36m|\033[1;37m   Selamat Datang di Sistem Belanja Online   \033[1;36m|\n";
        std::cout << "\033[1;36m+=============================================+\033[0m\n";

        std::vector<std::string> menu = {
            "1. Login",
            "2. Daftar (Registrasi)",
            "0. Keluar"
        };

        for (const auto& m : menu) {
            std::cout << "\033[1;36m|\033[0m " 
                      << std::left << std::setw(44) << m 
                      << "\033[1;36m|\033[0m\n";
        }

        std::cout << "\033[1;36m+=============================================+\033[0m\n";
        std::cout << "\033[1;33mPilihan: \033[0m";

        std::string input;
        std::getline(std::cin, input);

        if (input == "1") {
            menu_pembeli(user_table, root_barang);
        } else if (input == "2") {
            register_user(user_table, user_data_file);
        } else if (input == "0") {
            std::cout << "\033[1;32mKembali ke menu utama.\033[0m\n";
            break;
        } else {
            std::cout << "\033[1;31mPilihan tidak valid.\033[0m\n";
        }
    }
}

// Fungsi untuk konversi tree barang jadi vector (untuk cari dan tampil lebih mudah)
void flatten_barang(NodeBarang* root, std::vector<Barang>& daftar) {
    if (!root) return;
    flatten_barang(root->kiri, daftar);
    daftar.push_back(root->data);
    flatten_barang(root->kanan, daftar);
}

// fungsi untuk menampilkan daftar barang
void tampilkan_barang(const std::vector<Barang>& daftar) {
    std::cout << "\033[1;97m== Daftar Barang ==\033[0m\n";
    std::cout << "+------+----------------------+--------+------------+\n";
    std::cout << "| \033[1;97mID\033[0m   | \033[1;97mNama Barang\033[0m          | \033[1;97mStok\033[0m   | \033[1;97mHarga\033[0m      |\n";
    std::cout << "+------+----------------------+--------+------------+\n";

    for (const auto& b : daftar) {
        std::cout << "| "
                  << std::right << std::setw(4) << b.id << " | "
                  << std::left  << std::setw(20) << b.nama << " | "
                  << std::right << std::setw(6) << b.stok << " | "
                  << std::right << std::setw(10) << b.harga << " |\n";
    }

    std::cout << "+------+----------------------+--------+------------+\n";
}

// Fungsi untuk inisasliasi rute pengiriman
void inisialisasi_jaringan(Graph& g) {
    tambah_koneksi(g, "Jakarta", "Bandung");
    tambah_koneksi(g, "Bandung", "Yogyakarta");
    tambah_koneksi(g, "Yogyakarta", "Surabaya");
    tambah_koneksi(g, "Jakarta", "Semarang");
    tambah_koneksi(g, "Semarang", "Surabaya");
}

// Fungsi cari barang berdasarkan nama/kategori (kategori di sini diasumsikan bagian nama)
std::vector<Barang> cari_barang(const std::vector<Barang>& daftar, const std::string& keyword) {
    std::vector<Barang> hasil;
    std::string key_lower = keyword;
    std::transform(key_lower.begin(), key_lower.end(), key_lower.begin(), ::tolower);

    for (const auto& b : daftar) {
        std::string nama_lower = b.nama;
        std::transform(nama_lower.begin(), nama_lower.end(), nama_lower.begin(), ::tolower);
        if (nama_lower.find(key_lower) != std::string::npos) {
            hasil.push_back(b);
        }
    }
    return hasil;
}

// Fungsi tambah barang ke keranjang
void tambah_ke_keranjang(std::vector<KeranjangItem>& keranjang, const Barang& barang, int jumlah) {
    // Cek stok
    if (barang.stok < jumlah) {
        throw std::runtime_error("Stok barang tidak cukup.");
    }
    // Cek sudah ada di keranjang?
    auto it = std::find_if(keranjang.begin(), keranjang.end(), [&](const KeranjangItem& item){
        return item.barang.id == barang.id;
    });
    if (it != keranjang.end()) {
        it->jumlah += jumlah;
    } else {
        keranjang.push_back({barang, jumlah});
    }
}

// Fungsi hitung total harga keranjang
double total_harga(const std::vector<KeranjangItem>& keranjang) {
    double total = 0;
    for (const auto& item : keranjang) {
        total += item.barang.harga * item.jumlah;
    }
    return total;
}

// Fungsi tampilkan keranjang
void tampilkan_keranjang(const std::vector<KeranjangItem>& keranjang) {
    if (keranjang.empty()) {
        std::cout << "Keranjang kosong.\n";
        return;
    }

    std::cout << "\033[1;97m== Keranjang Anda ==\033[0m\n";
    std::cout << "+----------------------+--------+---------------+------------+\n";
    std::cout << "| \033[1;97mNama Barang\033[0m          | \033[1;97mJumlah\033[0m | \033[1;97mHarga Satuan\033[0m  | \033[1;97mSubtotal\033[0m   |\n";
    std::cout << "+----------------------+--------+---------------+------------+\n";

    for (const auto& item : keranjang) {
        double subtotal = item.barang.harga * item.jumlah;
        std::cout << "| "
                  << std::left  << std::setw(20) << item.barang.nama << " | "
                  << std::right << std::setw(6)  << item.jumlah       << " | "
                  << std::right << std::setw(13) << item.barang.harga << " | "
                  << std::right << std::setw(10) << subtotal << " |\n";
    }

    std::cout << "+----------------------+--------+---------------+------------+\n";
    std::cout << "Total Harga: \033[1;92m" << total_harga(keranjang) << "\033[0m\n";
}

void menu_pembeli(UserTable& user_table, NodeBarang* root_barang) {
    std::string username;
    if (!login_pembeli(user_table, username)) {
        std::cout << "\033[91mGagal login. Kembali ke menu utama.\033[0m\n";
        return;
    }

    std::vector<Barang> daftar_barang;
    flatten_barang(root_barang, daftar_barang);

    std::vector<KeranjangItem> keranjang;

    while (true) {
        std::cout << "\033[1;36m\n+===================================+\n";
        std::cout << "\033[1;36m|\033[1;37m           MENU PEMBELI            \033[1;36m|\n";
        std::cout << "\033[1;36m+===================================+\033[0m\n";

        std::vector<std::string> menu = {
            "1. Lihat semua barang",
            "2. Cari barang",
            "3. Tambah barang ke keranjang",
            "4. Lihat keranjang dan checkout",
            "0. Logout"
        };

        for (const auto& m : menu) {
            std::cout << "\033[1;36m|\033[0m "
                      << std::left << std::setw(34) << m 
                      << "\033[1;36m|\033[0m\n";
        }

        std::cout << "\033[1;36m+===================================+\033[0m\n";

        std::cout << "\033[1;33mPilihan: \033[0m";

        std::string input;
        std::getline(std::cin, input);

        try {
            int pilihan = safe_stoi(input);
            if (pilihan == 0) {
                std::cout << "\033[1;32mLogout berhasil.\033[0m\n";
                break;
            }
            else if (pilihan == 1) {
                tampilkan_barang(daftar_barang);
            }
            else if (pilihan == 2) {
                std::cout << "Masukkan kata kunci pencarian: ";
                std::string keyword;
                std::getline(std::cin, keyword);
                auto hasil = cari_barang(daftar_barang, keyword);
                if (hasil.empty()) {
                    std::cout << "\033[1;31mBarang tidak ditemukan.\033[0m\n";
                } else {
                    tampilkan_barang(hasil);
                }
            }
            else if (pilihan == 3) {
                std::cout << "Masukkan ID barang yang ingin ditambahkan: ";
                std::string id_str;
                std::getline(std::cin, id_str);
                int id = safe_stoi(id_str);
                auto it = std::find_if(daftar_barang.begin(), daftar_barang.end(),
                                       [id](const Barang& b){ return b.id == id; });
                if (it == daftar_barang.end()) {
                    throw std::runtime_error("Barang dengan ID tersebut tidak ditemukan.");
                }
                std::cout << "Masukkan jumlah: ";
                std::string jumlah_str;
                std::getline(std::cin, jumlah_str);
                int jumlah = safe_stoi(jumlah_str);
                if (jumlah <= 0) {
                    throw std::runtime_error("Jumlah harus positif.");
                }
                tambah_ke_keranjang(keranjang, *it, jumlah);
                std::cout << "\033[1;32mBerhasil menambahkan ke keranjang.\033[0m\n";
            }
            else if (pilihan == 4) {
                tampilkan_keranjang(keranjang);
                std::cout << "Checkout? (y/n): ";
                std::string cek;
                std::getline(std::cin, cek);
                if (cek == "y" || cek == "Y") {
                    if (keranjang.empty()) {
                        std::cout << "\033[1;31mKeranjang kosong, tidak bisa checkout.\033[0m\n";
                    } else {
                        // Biaya pengiriman dengan graph
                        Graph jaringan;
                        inisialisasi_jaringan(jaringan);

                        std::string asal;
                        std::string tujuan;
                        std::cout << "Masukkan kota asal pengiriman: ";
                        std::getline(std::cin, asal);
                        std::cout << "Masukkan kota tujuan pengiriman: ";
                        std::getline(std::cin, tujuan);

                        // Simulasi dengan dfs dan bfs
                        std::cout << "\033[1;34mSimulasi penelusuran (DFS): \033[0m\n";
                        dfs(jaringan, asal);
                        std::cout << "\033[1;34mSimulasi penelusuran (BFS): \033[0m\n";
                        bfs(jaringan, asal);

                        int jarak = estimasi_rute_bfs(jaringan, asal, tujuan);
                        if (jarak == -1) {
                            std::cout << "\033[1;31mTujuan tidak terjangkau dari " << asal << ". Pengiriman dibatalkan.\033[0m\n";
                            return;
                        }

                        int biaya_per_hop = 10000;
                        int ongkir = jarak * biaya_per_hop;
                        std::cout << "Estimasi pengiriman: " << jarak << " kota\n";
                        std::cout << "\033[1;32mBiaya kirim: Rp" << ongkir << "\033[0m\n";

                        //Konfirmasi lanjut
                        std::cout << "Lanjutkan Checkout dengan ongkir Rp" << ongkir << "? (y/n): ";
                        std::string lanjut;
                        std::getline(std::cin, lanjut);
                        if (lanjut != "y" && lanjut != "Y") {
                            std::cout << "\033[91mCheckout dibatalkan.\033[0m\n";
                            return;
                        }
                        // Update stok barang pada tree (kurangi stok sesuai keranjang)
                        for (const auto& item : keranjang) {
                            NodeBarang* node = cariBarang(root_barang, item.barang.id);
                            if (node) {
                                if (node->data.stok >= item.jumlah) {
                                    node->data.stok -= item.jumlah;
                                } else {
                                    std::cerr << "Stok barang " << node->data.nama << " tidak cukup saat checkout.\n";
                                }
                            }
                        }
                        simpan_data_barang(root_barang, "SistemBelanjaOnline/data/barang.txt");
                        std::cout << "\033[1;32mCheckout berhasil. Terima kasih telah berbelanja!\033[0m\n";
                        keranjang.clear();
                        // Reload daftar barang setelah stok berubah
                        daftar_barang.clear();
                        flatten_barang(root_barang, daftar_barang);
                    }
                }
            }
            else {
                std::cout << "Pilihan tidak valid.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
        }
    }
}
