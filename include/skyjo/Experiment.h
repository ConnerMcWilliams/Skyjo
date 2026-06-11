//
// Created by Conner on 5/30/2026.
//

// Experiment.h
#ifndef SKYJO_EXPERIMENT_H
#define SKYJO_EXPERIMENT_H

#include "Game.h"
#include "./agents/Agent.h"

#include <memory>
#include <vector>

struct ExperimentResult {
    int num_games = 0;
    std::vector<int> wins;
    std::vector<double> total_scores;
    std::vector<double> average_scores;
};

ExperimentResult RunExperiment(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents,
    int num_games
);

#endif //SKYJO_EXPERIMENT_H
