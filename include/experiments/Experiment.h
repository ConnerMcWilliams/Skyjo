//
// Created by Conner on 5/30/2026.
//

// Experiment.h
#ifndef SKYJO_EXPERIMENT_H
#define SKYJO_EXPERIMENT_H

#include "../skyjo/Game.h"
#include "../agents/Agent.h"

#include <memory>
#include <vector>

struct ExperimentResult {
    int num_games = 0;
    std::vector<int> wins;
    std::vector<double> total_scores;
    std::vector<double> average_scores;
};

/**
 * @brief Simulates n games with the specified array of agents with the rules given in game.
 *
 * @param game This is the game used to create each initial state.
 * @param agents This is the array of agents used in every game played.
 * @param num_games This is the number of games played.
 * @return Returns an ExperimentResult that includes for each agent the wins and scores.
 */
[[nodiscard]] ExperimentResult RunGames(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents,
    int num_games
);

/**
 * @brief Simulates n games and tracks the time taken and the wins and prints all the statistics.
 *
 * @param num_games The number of games played.
 * @param game This is the game used to create each initial state.
 * @param agents This is the array of agents used in every game played.
 */
void RunBasicExperiment(
    int num_games,
    const Game &game,
    std::vector<std::unique_ptr<Agent>> &agents
    );

#endif //SKYJO_EXPERIMENT_H
