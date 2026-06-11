//
// Created by Conner on 6/1/2026.
//

#ifndef SKYJO_BASICLOGIC_H
#define SKYJO_BASICLOGIC_H

#include "./Agent.h"
#include "../Game.h"
#include "../State.h"

#include <random>

class BasicLogic : public Agent {
public:
    explicit BasicLogic(double card_avg, unsigned int seed);

    [[nodiscard]] Action ChooseAction(const State& state) override;

private:
    const double card_avg_;
    std::mt19937 rng_;
};


#endif //SKYJO_BASICLOGIC_H
