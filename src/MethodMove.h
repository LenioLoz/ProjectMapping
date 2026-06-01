#ifndef CMAKESFMLPROJECT_METHODMOVE_H
#define CMAKESFMLPROJECT_METHODMOVE_H
#include <stack>
#include <vector>
#include "Wierzcholek.h"

class Agent;

class Move {
protected:
    std::vector<Vertex_>& graph;
public:
    Move(std::vector<Vertex_>& _graph):graph(_graph){};
    virtual void doMove(Agent& agent)=0;
    virtual ~Move() = default;
protected:
    void doSlide(Agent& agent, const Vertex_& nowyWierzcholek);
};

class RandomMove : public Move {
    bool isInitialized = false;
public:
    explicit RandomMove(std::vector<Vertex_>& _graph):Move(_graph){};
    void doMove(Agent& agent) override;

};

class DFS: public Move {
    stack <int> Stack;
    bool isInitialized = false;
public:
    explicit DFS(vector<Vertex_>& _graph):Move(_graph){};
    void doMove(Agent& agent) override;
};

#endif