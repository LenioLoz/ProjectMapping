#ifndef CMAKESFMLPROJECT_WIERZCHOLEK_H
#define CMAKESFMLPROJECT_WIERZCHOLEK_H
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;
class Agent;
class Strategia;
enum class StatusVertex {
    Blocked,
    Opened
};

class Vertex_ {
    float x;
    float y;
    vector<int> neighbour;
    int id;
    StatusVertex status;
    int agentCount = 0;
    static const int max_ = 2;

public:
    Vertex_(float _x, float _y, const vector<int>& _neighbour, const int _id): x(_x), y(_y), neighbour(_neighbour),id(_id) {
        status = StatusVertex::Opened;
    };
    [[nodiscard]] float getX()const{return x;}
    [[nodiscard]] float getY()const{return y;}
    [[nodiscard]] int getID()const {return id;}
    [[nodiscard]] bool isBlocked()const {return agentCount >= max_;}
    void comeAgent() {agentCount++;}
    void loseAgent() {
        if (agentCount > 0) {
            agentCount--;
        }
    }
    [[nodiscard]] StatusVertex getStatus()const{return status;}
    void setStatus(StatusVertex _status){status = _status;}
    [[nodiscard]] const vector<int>&  getNeighbour() const {return neighbour;}
    void Informacje() {
        cout<<"x = "<<x<<endl;
        cout<<"y = "<<y<<endl;
        cout<<"Sasiedzi= ";
        for (int n : neighbour) {
            cout<<n<<" ";
        }
    }
};

void Load_data(vector<int>& wierz, vector<float>& x, vector<float>& y, vector<vector<int>>& _neighbour);
void create_map(vector<Vertex_>& wierzcholki, vector<RectangleShape>& kwadraty, vector<vector<Vertex>>& line);
void interakcja (Agent& agent1,Agent& agent2, const std::vector<Vertex_>& graf);
void create_new_game(int liczbaAgentowDFS, int liczbaAgentowLosowy, vector<Agent>& agents, vector<Vertex_>& vertex, vector<Strategia*>& pulaStrategii);
int create_Graph();

#endif