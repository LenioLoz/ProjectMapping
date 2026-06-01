#include "TypyStrategii.h"
#include "Agent.h"

bool TitForTat::decyzjaAgenta(const Agent& agent1, const Agent& agent2) {
    auto historyEnter = agent1.getHistoria().find(agent2.getId());
    if (historyEnter == agent1.getHistoria().end()) {
        return true;
    }
    return historyEnter->second;
}

void TitForTat::updateHistory( Agent& agent,int idAgent, bool decision) {
    agent.setHistoria(idAgent, decision);
}
