//
// Created by Conner on 5/30/2026.
//

#include "../include/skyjo/Experiment.h"
#include "../include/skyjo/Simulation.h"

ExperimentResult RunExperiment(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents,
    int num_games
) {
    int num_players = game.NumPlayers();

    ExperimentResult result;
    result.num_games = num_games;
    result.wins.assign(num_players, 0);
    result.total_scores.assign(num_players, 0.0);
    result.average_scores.assign(num_players, 0.0);

    for (int g = 0; g < num_games; ++g) {
        GameResult game_result = RunSingleGame(game, agents);

        result.wins[game_result.winner]++;

        for (int p = 0; p < num_players; ++p) {
            result.total_scores[p] += game_result.scores[p];
        }
    }

    for (int p = 0; p < num_players; ++p) {
        result.average_scores[p] =
            result.total_scores[p] / static_cast<double>(num_games);
    }

    return result;
}
