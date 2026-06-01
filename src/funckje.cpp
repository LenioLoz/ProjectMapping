#include "Wierzcholek.h"
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include "Agent.h"
#include "TypyStrategii.h"
using namespace std;
using namespace sf;


void Load_data(vector<int>& wierz, vector<float>& x, vector<float>& y, vector<vector<int>>& sasiad) {
    fstream plik;
    plik.open("GraphStandard.txt");
    cout << endl;
    if (!plik.is_open()) {
        cerr<<"Can't open GrafStandard.txt"<<endl;
    }
    else {
        string line;
        float _x, _y;
        int _wierz;
        int sus;
        while (getline(plik, line)) {
            stringstream ss(line);
            if (ss >> _wierz >> _x >> _y) {
                x.push_back(_x);
                y.push_back(_y);
                wierz.push_back(_wierz);
                sasiad.emplace_back();
                while (ss >> sus) {
                    sasiad.back().push_back(sus);
                }
            }
        }
        plik.close();
    }
}

void create_map(vector<Vertex_>& wierzcholki, vector<RectangleShape>& kwadraty, vector<vector<Vertex>>& line) {
    vector<int> wierz; vector<float> x, y;
    vector<vector<int>> sasiad;
    Load_data(wierz, x, y, sasiad);

    for (int i = 0; i < wierz.size(); i++) {
        Vertex_ w(x[i], y[i], sasiad[i], wierz[i]);
        wierzcholki.push_back(w);
    }

    for (int i = 0; i < wierz.size(); i++) {
        RectangleShape rs;
        rs.setSize(Vector2f(10.f, 10.f));
        rs.setFillColor(Color::White);
        rs.setPosition(Vector2f(wierzcholki[i].getX() - 5.f, wierzcholki[i].getY()- 5.f));
        kwadraty.push_back(rs);
        for (int j = 0; j < wierzcholki[i].getNeighbour().size(); j++) {
            vector<Vertex> l = {
                Vertex{Vector2f(wierzcholki[i].getX(), wierzcholki[i].getY())},
                Vertex{Vector2f(wierzcholki[wierzcholki[i].getNeighbour()[j]].getX(), wierzcholki[wierzcholki[i].getNeighbour()[j]].getY())}
            };
            line.push_back(l);
        }
    }
    // Log diagnostyczny po zbudowaniu struktur do rysowania
    cout << "[create_map] wierzcholki=" << wierzcholki.size()
         << " kwadraty=" << kwadraty.size()
         << " linie=" << line.size() / 2 << endl;
}


void interakcja (Agent& agent1,Agent& agent2, const std::vector<Vertex_>& graf) {
    //std::cout<<"SPOTKANIE AGENTOW "<<agent1.getId()<<" I "<<agent2.getId()<<"\n"<<"Na wirzcholku "<<agent1.getCurrentVertex().getID()<<"\n";
    // std::cout << "PRZED INTERAKCJA\n";
    // std::cout << "Agent " << agent1.getId() << "  liczba punktow: " << agent1.getPoints()  << "\n";
    //std::cout << "Agent " << agent2.getId() << " liczba punktow: " << agent2.getPoints() << "\n";
    bool decyzjaA = agent1.getStrategia().decyzjaAgenta(agent1, agent2);
    bool decyzjaB = agent2.getStrategia().decyzjaAgenta(agent2, agent1);
    // std::cout <<"INTERAKCJA"<<"\n"<< "Agent " << agent1.getId() << (decyzjaA ? " KOOPERUJE" : " OSZUKUJE")
    //          << " vs Agent " << agent2.getId() << (decyzjaB ? " KOOPERUJE" : " OSZUKUJE") << "\n";
    if (decyzjaA && decyzjaB) {
        agent1.addPoints(2);
        agent2.addPoints(2);
    } else if (!decyzjaA && !decyzjaB) {}
    else if(decyzjaA && !decyzjaB){
        agent1.subPoints(1);
        agent2.addPoints(3);
    }
    else {
        agent1.addPoints(3);
        agent2.subPoints(1);
    }
   // std::cout << "[PO INTERAKCJI]\n";
   // std::cout << "Agent " << agent1.getId() << "  liczba punktow: " << agent1.getPoints()  << "\n";
   // std::cout << "Agent " << agent2.getId() << " liczba punktow: " << agent2.getPoints() << "\n";
    agent1.getStrategia().updateHistory(agent1, agent2.getId(), decyzjaB);
    agent2.getStrategia().updateHistory(agent2, agent1.getId(), decyzjaA);
}


void create_new_game(int liczbaAgentowDFS, int liczbaAgentowLosowy, vector<Agent>& agents, vector<Vertex_>& vertex, vector<Strategia*>& pulaStrategii) {
    int globalId = 0;
    vector<int> pozycje;
    for (int i = 0; i < liczbaAgentowDFS; i++) {
        int agresywnosc = std::rand() % 100 + 1;
        int indexStrategii = std::rand() % pulaStrategii.size();
        int indexWierzcholka = std::rand() % vertex.size();
        while (find(pozycje.begin(), pozycje.end(), indexWierzcholka) != pozycje.end()) {
            indexWierzcholka = std::rand() % vertex.size();
        }
        pozycje.push_back(indexWierzcholka);
        const Vertex_& losowyWierzcholek = vertex[indexWierzcholka];
        Agent aDFS(globalId++, agresywnosc,*pulaStrategii[indexStrategii],losowyWierzcholek, losowyWierzcholek, std::make_shared<DFS>(vertex), vertex.size(), "DFS");
        Color kolor = Color(rand() % 255, rand() % 255, rand() % 255);
        aDFS.setColor(kolor);
        agents.push_back(aDFS);
    }
    for (int i = 0; i < liczbaAgentowLosowy; i++) {
        int agresywnosc = std::rand() % 100 + 1;
        int indexStrategii = std::rand() % pulaStrategii.size();
        int indexWierzcholka = std::rand() % vertex.size();
        while (find(pozycje.begin(), pozycje.end(), indexWierzcholka) != pozycje.end()) {
            indexWierzcholka = std::rand() % vertex.size();
        }
        pozycje.push_back(indexWierzcholka);
        const Vertex_& losowyWierzcholek = vertex[indexWierzcholka];
        Agent aLosowy(globalId++, agresywnosc,*pulaStrategii[indexStrategii],losowyWierzcholek, losowyWierzcholek, std::make_shared<RandomMove>(vertex), vertex.size(), "RandomMove");
        Color kolor = Color(rand() % 255, rand() % 255, rand() % 255);
        aLosowy.setColor(kolor);
        agents.push_back(aLosowy);
    }

}

int  create_Graph() {
    std::cout << "Generowanie grafu python";
    std::cout << "\n";
    int systems = system("python3 GraphGenerator.py");
    if (systems != 0) {
        return 0;
    }
    return 1;
}