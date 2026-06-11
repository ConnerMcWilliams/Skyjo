//
// Created by Conner on 6/1/2026.
//

#ifndef SKYJO_MINIMAXAGENT_H
#define SKYJO_MINIMAXAGENT_H

#include "./Agent.h"
#include "../State.h"

class MinimaxAgent : public Agent {
public:
    explicit MinimaxAgent(int depth);

    [[nodiscard]] Action ChooseAction(const State& state) override;


private:
    int depth_;

private:
    [[nodiscard]] double EvalFunction(State state, int root_player) const;
    [[nodiscard]] double Search(State state, int root_player,
        int current_depth, double alpha, double beta) const;
    [[nodiscard]] double ChanceSearch(State state, int root_player,
        int current_depth, double alpha, double beta) const;
};

#endif //SKYJO_MINIMAXAGENT_H
