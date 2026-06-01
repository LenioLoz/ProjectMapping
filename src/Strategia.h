#ifndef STRATEGIA_H
#define STRATEGIA_H

#include <string>


class Agent;

class Strategia {
public:
    virtual ~Strategia() = default;
    virtual bool decyzjaAgenta(const Agent& agent1,const Agent& agent2 ) = 0;
    virtual std::string nazwaStrategii() const = 0;
    virtual void updateHistory(Agent &agent ,int idAgent, bool decision){}
};

#endif
