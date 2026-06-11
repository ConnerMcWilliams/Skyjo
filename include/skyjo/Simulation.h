//
// Created by Conner on 5/30/2026.
//

#ifndef SKYJO_SIMULATION_H
#define SKYJO_SIMULATION_H
#include <memory>
#include <vector>
#include "./Game.h"

class Agent;

struct GameResult {
    std::vector<int> scores;
    std::vector<double> returns;
    int winner = -1;
};

GameResult RunSingleGame(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents
);


#endif //SKYJO_SIMULATION_H
