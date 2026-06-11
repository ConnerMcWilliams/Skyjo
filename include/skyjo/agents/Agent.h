//
// Created by Conner on 5/30/2026.
//

#ifndef SKYJO_AGENT_H
#define SKYJO_AGENT_H
#include "../State.h"
#include "../Action.h"

class Agent {
public:
    virtual ~Agent() = default;

    [[nodiscard]] virtual Action ChooseAction(const State& state) = 0;
};


#endif //SKYJO_AGENT_H