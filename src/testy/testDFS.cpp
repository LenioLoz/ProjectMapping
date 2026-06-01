//
// Created by patry on 31.10.2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
using namespace std;

// ---------------------------
// Klasa Wierzcholek
// ---------------------------
class Wierzcholek {
    float x, y;
    vector<int> sasiedzi;
public:
    Wierzcholek(float _x, float _y, const vector<int>& _sasiedzi)
        : x(_x), y(_y), sasiedzi(_sasiedzi) {}
    Wierzcholek() : x(0), y(0) {}

    float getX() const { return x; }
    float getY() const { return y; }
    const vector<int>& getSasiedzi() const { return sasiedzi; }

    void Info() const {
        cout << "x=" << x << " y=" << y << " sasiedzi: ";
        for (int s : sasiedzi) cout << s << " ";
        cout << endl;
    }
};

// ---------------------------
// Funkcja do wczytania grafu
// ---------------------------
vector<Wierzcholek> wczytajGraf(const string& nazwaPliku) {
    ifstream plik("D:\Graf\GrafStandartVer.txt");
    vector<Wierzcholek> graf;

    if (!plik.is_open()) {
        cerr << "❌ Nie mozna otworzyc pliku: " << nazwaPliku << endl;
        return graf;
    }

    string linia;
    int id;
    float x, y;

    while (getline(plik, linia)) {
        stringstream ss(linia);
        if (!(ss >> id >> x >> y)) continue;

        vector<int> sasiedzi;
        int s;
        while (ss >> s)
            sasiedzi.push_back(s);

        graf.emplace_back(x, y, sasiedzi);
    }

    plik.close();
    return graf;
}

// ---------------------------
// Iteracyjny DFS
// ---------------------------
void DFS(const vector<Wierzcholek>& graf, int start) {
    int n = graf.size();
    vector<bool> odwiedzony(n, false);
    stack<int> S;
    S.push(start);

    cout << "🔍 Kolejnosc odwiedzania DFS od " << start << ":\n";

    while (!S.empty()) {
        int v = S.top();
        S.pop();

        if (!odwiedzony[v]) {
            odwiedzony[v] = true;
            cout << v << " ";

            // Dodajemy sąsiadów w odwrotnej kolejności, żeby zachować spójność
            // (bo stos przetwarza wierzchołki LIFO)
            const vector<int>& sasiedzi = graf[v].getSasiedzi();
            for (auto it = sasiedzi.rbegin(); it != sasiedzi.rend(); ++it) {
                if (!odwiedzony[*it])
                    S.push(*it);
            }
        }
    }

    cout << endl;
}

// ---------------------------
// Główny program
// ---------------------------
int main() {

    vector<Wierzcholek> graf = wczytajGraf("graf.txt");

    if (graf.empty()) {
        cerr << "Brak danych grafu!" << endl;
        return 1;
    }

    cout << "✅ Wczytano " << graf.size() << " wierzcholkow.\n";
    cout << "Pierwsze 3 dla sprawdzenia:\n";
    for (int i = 0; i < min(3, (int)graf.size()); i++) graf[i].Info();

    cout << "\n--- START DFS ---\n";
    DFS(graf, 0);  // Możesz zmienić 0 na dowolny startowy wierzchołek

    return 0;
}
