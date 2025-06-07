#include "include/pembeli_mode.hpp"
#include "include/exception_handler.hpp"
#include "include/graph_jaringan.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>

// Fungsi untuk registrasi pembeli baru
bool registrasi_pembeli(UserTable& user_table) {
    std::string username, password;
    std::cout << "=== Registrasi Pembeli ===\n";
    std::cout << "Username baru: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // Cek apakah username sudah digunakan
    if (user_table.find(username) != user_table.end()) {
        std::cerr << "Username sudah terdaftar. Silakan pilih username lain.\n";
        return false;
    }

    user_table[username] = password;
    std::cout << "Registrasi berhasil. Silakan login.\n";
    return true;
}

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

// Fungsi untuk konversi tree barang jadi vector (untuk cari dan tampil lebih mudah)
void flatten_barang(NodeBarang* root, std::vector<Barang>& daftar) {
    if (!root) return;
    flatten_barang(root->kiri, daftar);
    daftar.push_back(root->data);
    flatten_barang(root->kanan, daftar);
}

// Fungsi tampilkan daftar barang
void tampilkan_barang(const std::vector<Barang>& daftar) {
    std::cout << "ID\tNama\tStok\tHarga\n";
    for (const auto& b : daftar) {
        std::cout << b.id << "\t" << b.nama << "\t" << b.stok << "\t" << b.harga << "\n";
    }
}

// Fungsi untuk inisasliasi rute pengiriman
void inisialisasi_jaringan(Graph& g) {
    tambah_koneksi(g, "Jakarta", "Bandung");
    tambah_koneksi(g, "Bandung", "Yogyakarta");
    tambah_koneksi(g, "Yogyakarta", "Surabaya");
    tambah_koneksi(g, "Jakarta", "Semarang");
    tambah_koneksi(g, "Semarang", "Surabaya");
    // Tambahkan kota sesuai kebutuhan
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

// Fungsi estimasi pengiriman (sederhana, misal biaya tetap 10% dari total harga)
double estimasi_pengiriman(const std::vector<KeranjangItem>& keranjang) {
    return total_harga(keranjang) * 0.1;
}

// Fungsi tampilkan keranjang
void tampilkan_keranjang(const std::vector<KeranjangItem>& keranjang) {
    if (keranjang.empty()) {
        std::cout << "Keranjang kosong.\n";
        return;
    }
    std::cout << "Keranjang Anda:\n";
    std::cout << "Nama\tJumlah\tHarga Satuan\tSubtotal\n";
    for (const auto& item : keranjang) {
        std::cout << item.barang.nama << "\t" << item.jumlah << "\t"
                  << item.barang.harga << "\t\t"
                  << item.barang.harga * item.jumlah << "\n";
    }
    std::cout << "Total Harga: " << total_harga(keranjang) << "\n";
    std::cout << "Estimasi Pengiriman (10%): " << estimasi_pengiriman(keranjang) << "\n";
    std::cout << "Total Bayar: " << total_harga(keranjang) + estimasi_pengiriman(keranjang) << "\n";
}

void menu_pembeli(UserTable& user_table, NodeBarang* root_barang) {
    std::string username;

    while (true) {
        std::cout << "\n=== Menu Awal Pembeli ===\n";
        std::cout << "1. Login\n";
        std::cout << "2. Registrasi\n";
        std::cout << "0. Kembali ke menu utama\n";
        std::cout << "Pilihan: ";
        std::string input;
        std::getline(std::cin, input);

        try {
            int pilihan = safe_stoi(input);
            if (pilihan == 1) {
                if (login_pembeli(user_table, username)) break;
            } else if (pilihan == 2) {
                registrasi_pembeli(user_table);
            } else if (pilihan == 0) {
                std::cout << "Kembali ke menu utama.\n";
                return;
            } else {
                std::cout << "Pilihan tidak valid.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    // LOGIN BERHASIL, lanjutkan ke menu pembeli:
    std::vector<Barang> daftar_barang;
    flatten_barang(root_barang, daftar_barang);

    std::vector<KeranjangItem> keranjang;

    while (true) {
        std::cout << "\n=== Menu Pembeli ===\n";
        std::cout << "1. Lihat semua barang\n";
        std::cout << "2. Cari barang\n";
        std::cout << "3. Tambah barang ke keranjang\n";
        std::cout << "4. Lihat keranjang dan checkout\n";
        std::cout << "0. Logout\n";
        std::cout << "Pilihan: ";

        std::string input;
        std::getline(std::cin, input);

        try {
            int pilihan = safe_stoi(input);
            if (pilihan == 0) {
                std::cout << "Logout berhasil.\n";
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
                    std::cout << "Barang tidak ditemukan.\n";
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
                std::cout << "Berhasil menambahkan ke keranjang.\n";
            }
            else if (pilihan == 4) {
                tampilkan_keranjang(keranjang);
                std::cout << "Checkout? (y/n): ";
                std::string cek;
                std::getline(std::cin, cek);
                if (cek == "y" || cek == "Y") {
                    if (keranjang.empty()) {
                        std::cout << "Keranjang kosong, tidak bisa checkout.\n";
                    } else {
                        Graph jaringan;
                        inisialisasi_jaringan(jaringan);
                        std::string asal, tujuan;
                        std::cout << "Masukkan kota asal pengiriman: ";
                        std::getline(std::cin, asal);
                        std::cout << "Masukkan kota tujuan pengiriman: ";
                        std::getline(std::cin, tujuan);

                        dfs(jaringan, asal);
                        int jarak = estimasi_rute_bfs(jaringan, asal, tujuan);
                        if (jarak == -1) {
                            std::cout << "Tujuan tidak terjangkau dari " << asal << ". Pengiriman dibatalkan\n";
                            return;
                        }

                        int biaya_per_hop = 10000;
                        int ongkir = jarak * biaya_per_hop;
                        std::cout << "Estimasi pengiriman: " << jarak << " kota\n";
                        std::cout << "Biaya kirim: Rp" << ongkir << "\n";

                        std::cout << "Lanjutkan Checkout dengan ongkir Rp" << ongkir << "? (y/n): ";
                        std::string lanjut;
                        std::getline(std::cin, lanjut);
                        if (lanjut != "y" && lanjut != "Y") {
                            std::cout << "Checkout dibatalkan.\n";
                            return;
                        }

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
                        std::cout << "Checkout berhasil. Terima kasih telah berbelanja!\n";
                        keranjang.clear();
                        daftar_barang.clear();
                        flatten_barang(root_barang, daftar_barang);
                    }
                }
            }
            else {
                std::cout << "Pilihan tidak valid.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}
