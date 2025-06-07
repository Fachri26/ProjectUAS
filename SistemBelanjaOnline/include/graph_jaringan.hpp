// graph_jaringan.h
#ifndef GRAPH_JARINGAN_H
#define GRAPH_JARINGAN_H

#include <string>
#include <vector>
#include <map>

// Tipe alias untuk graph adjacency list
using Graph = std::map<std::string, std::vector<std::string>>;

// Fungsi
void tambah_koneksi(Graph& graph, const std::string& dari, const std::string& ke);
void bfs(const Graph& graph, const std::string& mulai);
void dfs(const Graph& graph, const std::string& mulai);

int estimasi_rute_bfs(const Graph& graph, const std::string& asal, const std::string& tujuan);


#endif // GRAPH_JARINGAN_H