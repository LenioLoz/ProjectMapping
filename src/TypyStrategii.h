#ifndef TYPY_STRATEGII_H
#define TYPY_STRATEGII_H
#include "Strategia.h"
#include <string>

class Agent;

class AlwaysCooperate : public Strategia {
public:
    bool decyzjaAgenta(const Agent& agent1,const Agent& agent2 ) override { return true; } // Zawsze współpracuj
    std::string nazwaStrategii() const override {
        return "Always Cooperate";
    }
};

class AlwaysDefect : public Strategia {
public:
    bool decyzjaAgenta(const Agent& agent1,const Agent& agent2 ) override { return false; } // Zawsze defektuj
    std::string nazwaStrategii() const override {
        return "Always Defect";
    }
};

class TitForTat : public Strategia {
public:
    bool decyzjaAgenta(const Agent& agent1,const Agent& agent2 ) override;
    std::string nazwaStrategii() const override {
        return "Tit For Tat";
    }
    void updateHistory(Agent& agent,int idAgent, bool decision) override;
};
#endif