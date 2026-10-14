//
// Created by Conner on 5/30/2026.
//

#ifndef SKYJO_AGENT_H
#define SKYJO_AGENT_H
#include "../skyjo/Action.h"
#include "../skyjo/State.h"

class Agent {
public:
    virtual ~Agent() = default;

    /// @returns Returns a legal action based on the state
    [[nodiscard]] virtual Action ChooseAction(const State& state) = 0;
};


#endif //SKYJO_AGENT_H