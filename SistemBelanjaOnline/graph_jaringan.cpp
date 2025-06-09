#include "include/graph_jaringan.hpp"
#include <iostream>
#include <queue>
#include <stack>
#include <set>

// Menambahkan koneksi dua arah antara lokasi
void tambah_koneksi(Graph& graph, const std::string& dari, const std::string& ke) {
    graph[dari].push_back(ke);
    graph[ke].push_back(dari); 
}

// Breadth-First Search
void bfs(const Graph& graph, const std::string& mulai) {
    std::set<std::string> dikunjungi;
    std::queue<std::string> q;
    q.push(mulai);
    dikunjungi.insert(mulai);

    std::cout << "BFS dari lokasi: " << mulai << "\n";
    while (!q.empty()) {
        std::string saat_ini = q.front();
        q.pop();
        std::cout << saat_ini << " ";

        for (const auto& tetangga : graph.at(saat_ini)) {
            if (dikunjungi.find(tetangga) == dikunjungi.end()) {
                q.push(tetangga);
                dikunjungi.insert(tetangga);
            }
        }
    }
    std::cout << "\n";
}

// Depth-First Search
void dfs(const Graph& graph, const std::string& mulai) {
    std::set<std::string> dikunjungi;
    std::stack<std::string> s;
    s.push(mulai);

    std::cout << "DFS dari lokasi: " << mulai << "\n";
    while (!s.empty()) {
        std::string saat_ini = s.top();
        s.pop();

        if (dikunjungi.find(saat_ini) == dikunjungi.end()) {
            std::cout << saat_ini << " ";
            dikunjungi.insert(saat_ini);

            for (auto it = graph.at(saat_ini).rbegin(); it != graph.at(saat_ini).rend(); ++it) {
                if (dikunjungi.find(*it) == dikunjungi.end()) {
                    s.push(*it);
                }
            }
        }
    }
    std::cout << "\n";
}

// Cari rute terpendek dari asal ke tujuan menggunakan BFS
int estimasi_rute_bfs(const Graph& graph, const std::string& asal, const std::string& tujuan) {
    std::set<std::string> dikunjungi;
    std::queue<std::pair<std::string, int>> q;
    q.push({asal, 0});
    dikunjungi.insert(asal);

    while (!q.empty()) {
        auto [saat_ini, jarak] = q.front();
        q.pop();

        if (saat_ini == tujuan) return jarak;

        for (const auto& tetangga : graph.at(saat_ini)) {
            if (dikunjungi.find(tetangga) == dikunjungi.end()) {
                q.push({tetangga, jarak + 1});
                dikunjungi.insert(tetangga);
            }
        }
    }

    return -1; // tidak ditemukan
}
