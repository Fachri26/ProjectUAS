#ifndef DATA_BARANG_H
#define DATA_BARANG_H

#include <string>
#include <functional>

// Struktur untuk barang
struct Barang {
    int id;
    std::string nama;
    int stok;
    double harga;
};

// Node AVL Tree
struct NodeBarang {
    Barang data;
    NodeBarang* kiri;
    NodeBarang* kanan;
    int tinggi;
};

// Deklarasi fungsi
NodeBarang* insertBarang(NodeBarang* root, Barang data);
NodeBarang* hapusBarang(NodeBarang* root, int id);
void inOrder(NodeBarang* root, std::function<void(Barang)> func);
NodeBarang* cariBarang(NodeBarang* root, int id);

// Fungsi AVL
int tinggi(NodeBarang* node);
int getBalance(NodeBarang* node);
NodeBarang* rotasiKanan(NodeBarang* y);
NodeBarang* rotasiKiri(NodeBarang* x);

#endif // DATA_BARANG_H