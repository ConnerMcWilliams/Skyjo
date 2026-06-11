//
// Created by Conner on 5/30/2026.
//

#include "../../include/skyjo/agents/RandomAgent.h"

#include <random>
#include "../../include/skyjo/State.h"

RandomAgent::RandomAgent(unsigned int seed) : rng_(seed) {}

Action RandomAgent::ChooseAction(const State& state) {
    const auto legal_actions = state.LegalActions();

    std::uniform_int_distribution<std::size_t> dist(
        0,
        legal_actions.size() - 1
    );

    return legal_actions[dist(rng_)];
}