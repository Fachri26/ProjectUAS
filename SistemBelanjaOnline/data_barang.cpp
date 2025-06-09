// data_barang.cpp
#include "include/data_barang.hpp"
#include <iostream>
#include <algorithm>

// Fungsi untuk mendapatkan tinggi node
int tinggi(NodeBarang* node) {
    if (!node) return 0;
    return node->tinggi;
}

// Fungsi untuk mendapatkan balance factor
int getBalance(NodeBarang* node) {
    if (!node) return 0;
    return tinggi(node->kiri) - tinggi(node->kanan);
}

// Rotasi kanan
NodeBarang* rotasiKanan(NodeBarang* y) {
    NodeBarang* x = y->kiri;
    NodeBarang* T2 = x->kanan;

    x->kanan = y;
    y->kiri = T2;

    y->tinggi = std::max(tinggi(y->kiri), tinggi(y->kanan)) + 1;
    x->tinggi = std::max(tinggi(x->kiri), tinggi(x->kanan)) + 1;

    return x;
}

// Rotasi kiri
NodeBarang* rotasiKiri(NodeBarang* x) {
    NodeBarang* y = x->kanan;
    NodeBarang* T2 = y->kiri;

    y->kiri = x;
    x->kanan = T2;

    x->tinggi = std::max(tinggi(x->kiri), tinggi(x->kanan)) + 1;
    y->tinggi = std::max(tinggi(y->kiri), tinggi(y->kanan)) + 1;

    return y;
}

// Fungsi insert
NodeBarang* insertBarang(NodeBarang* root, Barang data) {
    if (!root) {
        NodeBarang* baru = new NodeBarang{data, nullptr, nullptr, 1};
        return baru;
    }
    if (data.id < root->data.id)
        root->kiri = insertBarang(root->kiri, data);
    else if (data.id > root->data.id)
        root->kanan = insertBarang(root->kanan, data);
    else
        return root; // duplikat ID tidak diperbolehkan

    root->tinggi = std::max(tinggi(root->kiri), tinggi(root->kanan)) + 1;

    int balance = getBalance(root);

    // Left Left
    if (balance > 1 && data.id < root->kiri->data.id)
        return rotasiKanan(root);

    // Right Right
    if (balance < -1 && data.id > root->kanan->data.id)
        return rotasiKiri(root);

    // Left Right
    if (balance > 1 && data.id > root->kiri->data.id) {
        root->kiri = rotasiKiri(root->kiri);
        return rotasiKanan(root);
    }

    // Right Left
    if (balance < -1 && data.id < root->kanan->data.id) {
        root->kanan = rotasiKanan(root->kanan);
        return rotasiKiri(root);
    }

    return root;
}

// Fungsi cari
NodeBarang* cariBarang(NodeBarang* root, int id) {
    if (!root || root->data.id == id)
        return root;
    if (id < root->data.id)
        return cariBarang(root->kiri, id);
    return cariBarang(root->kanan, id);
}

// Fungsi inorder traversal
void inOrder(NodeBarang* root, std::function<void(Barang)> func) {
    if (root) {
        inOrder(root->kiri, func);
        func(root->data);
        inOrder(root->kanan, func);
    }
}

// Fungsi minimum node
NodeBarang* minValueNode(NodeBarang* node) {
    NodeBarang* current = node;
    while (current->kiri)
        current = current->kiri;
    return current;
}

// Fungsi hapus
NodeBarang* hapusBarang(NodeBarang* root, int id) {
    if (!root) return root;

    if (id < root->data.id)
        root->kiri = hapusBarang(root->kiri, id);
    else if (id > root->data.id)
        root->kanan = hapusBarang(root->kanan, id);
    else {
        if (!root->kiri || !root->kanan) {
            NodeBarang* temp = root->kiri ? root->kiri : root->kanan;
            if (!temp) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            NodeBarang* temp = minValueNode(root->kanan);
            root->data = temp->data;
            root->kanan = hapusBarang(root->kanan, temp->data.id);
        }
    }

    if (!root) return root;

    root->tinggi = std::max(tinggi(root->kiri), tinggi(root->kanan)) + 1;

    int balance = getBalance(root);

    // Rebalance
    if (balance > 1 && getBalance(root->kiri) >= 0)
        return rotasiKanan(root);
    if (balance > 1 && getBalance(root->kiri) < 0) {
        root->kiri = rotasiKiri(root->kiri);
        return rotasiKanan(root);
    }
    if (balance < -1 && getBalance(root->kanan) <= 0)
        return rotasiKiri(root);
    if (balance < -1 && getBalance(root->kanan) > 0) {
        root->kanan = rotasiKanan(root->kanan);
        return rotasiKiri(root);
    }

    return root;
}
