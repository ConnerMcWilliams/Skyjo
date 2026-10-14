//
// Created by Conner on 5/30/2026.
//

#include "../../include/experiments/Experiment.h"

#include <chrono>
#include <fstream>

#include "../../include/experiments/Simulation.h"

void RunBasicExperiment(const int num_games, const Game &game, std::vector<std::unique_ptr<Agent>> &agents) {
    using Clock = std::chrono::steady_clock;

    const auto start = Clock::now();

    const ExperimentResult result = RunGames(game, agents, num_games);

    const auto end = Clock::now();

    const std::chrono::duration<double> elapsed_seconds = end - start;
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    const double games_per_second =
        result.num_games / elapsed_seconds.count();

    std::ofstream out("./NormalExperiment.txt");

    out << std::fixed << std::setprecision(3);

    out << "Games played: " << result.num_games << "\n";
    out << "Elapsed time: " << elapsed_ms << " ms\n";
    out << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";
    out << "Speed: " << games_per_second << " games/second\n\n";

    for (int p = 0; p < game.NumPlayers(); ++p) {
        out << "Agent " << p << ":\n";
        out << "  Wins: " << result.wins[p] << "\n";
        out << "  Average score: " << result.average_scores[p] << "\n\n";
    }
}

ExperimentResult RunGames(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents,
    const int num_games
) {
    const int num_players = game.NumPlayers();

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
