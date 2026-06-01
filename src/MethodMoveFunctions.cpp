#include "MethodMove.h"
#include <vector>
#include "Agent.h"
#include "Wierzcholek.h"
#include <algorithm>

void Move::doSlide(Agent &agent, const Vertex_ &nowyWierzcholek) {
    if (agent.getCurrentVertex().getID() == nowyWierzcholek.getID()) {
        return;
    }
    graph[agent.getCurrentVertex().getID()].loseAgent();
    graph[nowyWierzcholek.getID()].comeAgent();
    agent.setCurrentVertex(nowyWierzcholek);
}

void DFS::doMove(Agent& agent) {
    if (!isInitialized) {
        isInitialized = true;
        vector<bool>& visited = agent.getVisited();
        Vertex_& current = agent.getCurrentVertex();
        Stack.push(current.getID());
        visited[current.getID()] = true;
        graph[current.getID()].comeAgent();
    }

    if (isInitialized && Stack.empty()) {
        agent.setStatus(AgentStatus::ENDED);
        return;
    }

    Vertex_& current = agent.getCurrentVertex();
    const vector<int>& neighbors = current.getNeighbour();
    vector<int> possibleMoves;
    vector<bool>visited = agent.getVisited();
    vector<int> notVisited_blocked;
    for (int neighbor: neighbors) {
        if (!visited[neighbor]) {
            if (!graph[neighbor].isBlocked()) {
                possibleMoves.push_back(neighbor);
            }
            else {
                notVisited_blocked.push_back(neighbor);
            }
        }
    }

    if (!possibleMoves.empty()) {
        Vertex_ next = graph[possibleMoves[0]];
        vector<bool>& visited = agent.getVisited();
        visited[next.getID()] = true;
        Stack.push(next.getID());
        agent.addPoints(1);
        //std::cout << "DFS [Agent " << agent.getId() << "] Ide do " << next.getID() << std::endl;
        agent.addKroki(1);
        doSlide(agent, next);
    }
    else {
        if (!notVisited_blocked.empty()) {
            //std::cout << "DFS [Agent " << agent.getId() << "] Czekam na " << current.getID() << " (zablokowane)" << std::endl;
            agent.licznikBezRuchu += 1;// ...
        } else {
            if (Stack.empty()) return;

            // Zdejmujemy aktualny wierzchołek ze stosu (bo nie ma stąd wyjścia)
            Stack.pop();

            if (Stack.empty()) {
                agent.setStatus(AgentStatus::ENDED);
                // graph[agent.getCurrentVertex().getID()].loseAgent(); // To jest ryzykowne, lepiej użyć ID z agenta
                if (agent.getCurrentVertex().getID() < graph.size()) {
                    graph[agent.getCurrentVertex().getID()].loseAgent();
                }
                return;
            }

            // Sprawdzamy co jest teraz na górze (gdzie wracamy)
            int previousID = Stack.top();

            // ZABEZPIECZENIE
            if (previousID < 0 || previousID >= graph.size()) {
                std::cerr << "BLAD STOSU: ID " << previousID << " poza zakresem grafu!" << std::endl;
                agent.setStatus(AgentStatus::ENDED); // Awaryjne zakończenie
                return;
            }

            Vertex_ previous = graph[previousID];

            if (previous.isBlocked()) {
                // Musimy przywrócić stan stosu, bo nie możemy się cofnąć
                // Ale w Twoim kodzie pushowałeś 'currentID', który zdjąłeś.
                // Logicznie w DFS cofasz się do 'previous', a nie zostajesz w 'current'.
                // Jeśli previous jest zablokowane, agent utknął.

                // Stack.push(currentID); // <-- To w Twoim kodzie było dziwne logicznie przy backtrackingu

            //    std::cout << "DFS [Agent " << agent.getId() << "] Czekam na " << current.getID() << " (zablokowane)" << std::endl;
                agent.licznikBezRuchu += 1;
                // Musimy naprawić stos, bo zdjęliśmy wierzchołek, a fizycznie się nie ruszyliśmy
                Stack.push(current.getID());
                return;
            }

            agent.addKroki(1);
          //  std::cout << "DFS [Agent " << agent.getId() << "] Cofam sie do " << previous.getID() << std::endl;
            doSlide(agent, previous);
        }
    }
}

void RandomMove::doMove(Agent& agent) {

    if (!isInitialized) {
        isInitialized = true;
        Vertex_& current = agent.getCurrentVertex();
        agent.getVisited()[current.getID()] = true;
        graph[current.getID()].comeAgent();
    }
    vector<bool>& visited = agent.getVisited();
    if (std::count(visited.begin(), visited.end(), true) == graph.size()) {
        agent.setStatus(AgentStatus::ENDED);
        graph[agent.getCurrentVertex().getID()].loseAgent();
        return;
    }
    Vertex_& current = agent.getCurrentVertex();
    const vector<int>& neighbors = current.getNeighbour();
    if (neighbors.empty()) {
        return;
    }
    vector<int> notVisited_Open;
    vector<int> visited_Open;
    for (int sasiadId : neighbors) {
        // ZABEZPIECZENIE PRZED SIGSEGV:
        if (sasiadId < 0 || sasiadId >= graph.size()) {
            std::cerr << "BLAD: Wierzcholek " << current.getID()
                     << " ma nieprawidlowego sasiada ID: " << sasiadId << std::endl;
            continue;
        }

        if (!graph[sasiadId].isBlocked()) { // Tutaj był potencjalny crash
            if (!visited[sasiadId]) {
                notVisited_Open.push_back(sasiadId);
            } else {
                visited_Open.push_back(sasiadId);
            }
        }
    }

    if (!notVisited_Open.empty()) {
        int losowyId = notVisited_Open[rand() % notVisited_Open.size()];
        Vertex_ nastepnyRuch = graph[losowyId];
        visited[nastepnyRuch.getID()] = true;
        agent.addPoints(1);
       // std::cout << "RandomWalk [Agent " << agent.getId() << "] Ide na " << nastepnyRuch.getID() << std::endl;
        agent.addKroki(1);
        doSlide(agent, nastepnyRuch);
    } else if (!visited_Open.empty()) {
        int losowyId = visited_Open[rand() % visited_Open.size()];
        Vertex_ nastepnyRuch = graph[losowyId];
        //std::cout << "RandomWalk [Agent " << agent.getId() << "] Wracam na " << nastepnyRuch.getID() << std::endl;
        agent.addKroki(1);
        doSlide(agent, nastepnyRuch);
    } else {
       // std::cout << "RandomWalk [Agent " << agent.getId() << "] Czekam na " << current.getID() << " (zablokowane)" << std::endl;
        agent.licznikBezRuchu += 1;
    }
}