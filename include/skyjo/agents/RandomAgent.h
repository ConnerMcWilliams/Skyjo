//
// Created by Conner on 5/30/2026.
//

#ifndef SKYJO_RANDOMAGENT_H
#define SKYJO_RANDOMAGENT_H
#include "./Agent.h"


class RandomAgent : public Agent {
public:
    explicit RandomAgent(unsigned int seed = std::random_device{}());

    [[nodiscard]] Action ChooseAction(const State& state) override;

private:
    std::mt19937 rng_;
};


#endif //SKYJO_RANDOMAGENT_H
