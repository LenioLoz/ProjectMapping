#ifndef SRC_AGENT_H
#define SRC_AGENT_H
#include <random>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Wierzcholek.h"
#include <vector>
#include "MethodMove.h"
#include <cmath> // Dodane dla animacji
#include <map>
#include "Strategia.h"




enum class AgentStatus {
    SEARCHING,
    ENDED,
    BLOCKED,
};

class Agent {
    int id;
    int points;
    int aggressive;
    AgentStatus status;
    vector<bool> visited;
    Strategia* strategia_ = nullptr;
    Vertex_ startVertex;
    Vertex_ current; // Pozycja LOGICZNA
    std::shared_ptr<Move> moveMethod;
    sf::Color color;
    int kroki = 0;
    // Pola dla animacji
    sf::Vector2f visualPosition;
    sf::Vector2f targetPosition;
    bool isMoving = false;
    float moveSpeed = 200.f; // Prędkość w pikselach na sekundę
    string nazwaAlgorytmu_ruchu;
    std::map<int, bool> historia;

public:
    int licznikBezRuchu = 0;
    Agent(int _id, int _aggressive, Strategia& _strategia, const Vertex_& _startVertex, const Vertex_& _currentVertex,std::shared_ptr<Move> _method ,const int size,  string _nazwa):
    id(_id), aggressive(_aggressive), strategia_(&_strategia), startVertex(_startVertex), current(_currentVertex),moveMethod(std::move(_method)),
    visualPosition(_startVertex.getX(), _startVertex.getY()),
    targetPosition(_startVertex.getX(), _startVertex.getY()),
    isMoving(false), nazwaAlgorytmu_ruchu(_nazwa)
    {
        points = 0;
        status = AgentStatus::SEARCHING;
        visited.resize(size+1, false);
    }

    int getId() const {return id;}
    const std::map<int, bool>& getHistoria() const { return historia; }
    void setHistoria(int idAgent, bool decision) { historia[idAgent] = decision; }
    void printHistoria() const {
        std::cout << "Historia agenta " << id << ":\n";
        for (const auto& entry : historia) {
            std::cout << "Agent ID: " << entry.first<<"Strategia: "<<this->getStrategia().nazwaStrategii() << ", Decyzja: " << (entry.second ? "Kooperuje" : "Oszukuje") << "\n";
        }
    }
    void setColor(sf::Color c) {color = c;}
    void addPoints(int p) {points += p;}
    string getAlgorytm(){ return nazwaAlgorytmu_ruchu;}
    AgentStatus getStatus() const {return status;}
    void setStatus(AgentStatus _status) {status = _status;}
    void subPoints(int p) {points -= p;}
    void addKroki(int _krok) {kroki += _krok;}
    int getKroki() const {return kroki;}
    int getPoints() const {return points;}
    int getAggressive() const {return aggressive;}
    Strategia& getStrategia() const{return *strategia_;}
    Vertex_& getStartVertex(){return startVertex;}
    Vertex_& getCurrentVertex(){return current;}
    vector<bool>& getVisited(){return visited;}
    bool getIsMoving() const { return isMoving; }
    void setVisited(vector<bool> _visited) {visited = _visited;}
    void setCurrent(Vertex_ _currentVertex) {
        current = _currentVertex;
        startVertex = _currentVertex;
        points = 0;
        kroki = 0;
    }

    void setCurrentVertex(Vertex_ _vertex){
        current = _vertex; // Ustaw pozycję logiczną
        targetPosition = sf::Vector2f(current.getX(), current.getY());
        isMoving = true; // Rozpocznij animację
    }

    void draw (sf::RenderWindow& window) const{
        sf::RectangleShape rs;
        rs.setSize(sf::Vector2f(10.f, 10.f));
        rs.setFillColor(color);
        rs.setPosition(sf::Vector2f(visualPosition.x -5.f, visualPosition.y -5.f));
        window.draw(rs);
    }
    void update(float deltaTime) {
        if (!isMoving) {
            return;
        }
        sf::Vector2f direction = targetPosition - visualPosition;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float moveAmount = moveSpeed * deltaTime;

        if (moveAmount >= distance) {
            visualPosition = targetPosition;
            isMoving = false;
        } else {
            visualPosition += (direction / distance) * moveAmount;
        }
    }


    void doMove() {
        if (moveMethod) {
            moveMethod -> doMove(*this);
        }
    }
};
#endif //SRC_AGENT_H