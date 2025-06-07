// file_handler.h
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "data_barang.hpp"
#include "data_user.hpp"
#include "graph_jaringan.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <map>

// Fungsi untuk memuat dan menyimpan barang
void simpan_data_barang(NodeBarang* root, const std::string& filename);
NodeBarang* muat_data_barang(const std::string& filename);

// Fungsi untuk memuat dan menyimpan user
void simpan_data_user(const UserTable& table, const std::string& filename);
void muat_data_user(UserTable& table, const std::string& filename);

// Fungsi untuk memuat dan menyimpan jaringan
void simpan_graph(const Graph& graph, const std::string& filename);
void muat_graph(Graph& graph, const std::string& filename);

#endif // FILE_HANDLER_H