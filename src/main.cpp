#include <algorithm>
#include <fstream>
#include "Wierzcholek.h"
#include "Agent.h"
#include <vector>
#include "TypyStrategii.h"


int main() {
    if (create_Graph() == 0) {
        std::cerr << "Graph creation failed!" << std::endl;
    } else {
        srand(static_cast<unsigned int>(time(nullptr)));
        int liczbaAgentowDFS;
        int liczbaAgentowLosowy;
        AlwaysCooperate coop;
        AlwaysDefect defect;
        TitForTat tft;
        std::ofstream plikout;

        plikout.open("Dane18AgentowGraf-TEST.txt");
        vector<Strategia*> pulaStrategii = {&coop, &defect, &tft};
        std::cout << "Podaj liczbe agentow DFS:";
        std::cin >> liczbaAgentowDFS;
        cout<<std::endl;
        std::cout << "Podaj liczbe agentow LOSOWYCH: ";
        std::cin >> liczbaAgentowLosowy;
        cout<<std::endl;
        vector<Vertex_> vertex;
        vector<Agent> agents;
        vector<RectangleShape> squares;
        vector<vector<Vertex>> line;
        create_map(vertex, squares, line);
        create_new_game(liczbaAgentowDFS, liczbaAgentowLosowy, agents, vertex, pulaStrategii);
        std::sort(agents.begin(), agents.end(), [](const Agent& agent1, const Agent& agent2) {
        return agent1.getAggressive() > agent2.getAggressive();});
        bool somebodyEnds = false;
        while (somebodyEnds == false)
        {
            for (Agent& agent : agents) {
                if (agent.getStatus() == AgentStatus::ENDED) {
                    if (somebodyEnds == false) {
                        somebodyEnds = true;
                        cout << "Agent " << agent.getId() <<" "<<agent.getAlgorytm() << " ukonczyl jako pierwszy" << endl;
                        agent.addPoints(5);
                    }
                    continue;
                }
                if (somebodyEnds) {
                    agent.setStatus(AgentStatus::ENDED);
                }else {
                    agent.doMove();
                }
            }
            for (size_t i = 0; i < agents.size(); ++i) {
                for (size_t j = i + 1; j < agents.size(); ++j) {
                    Agent& a = agents[j];
                    Agent& b = agents[i];
                    if (a.getCurrentVertex().getID() == b.getCurrentVertex().getID()
                        && a.getStatus() != AgentStatus::ENDED && b.getStatus() != AgentStatus::ENDED) {
                        for (int l = 0; l < 10; l++)
                            interakcja(a, b, vertex);
                        }
                }
            }
        }
        // cout<<"Punkty Agentow: " << endl;
        // for (Agent& agent : agents) {
        //     cout<<"Agent "<<agent.getId() << " zdobyl: "<< agent.getPoints() <<
        //         " Taktyka agenta: "<<agent.getStrategia().nazwaStrategii() <<" Agresywnosc agenta: "<<agent.getAggressive()<<endl;
        // }
    }
    return 0;
}