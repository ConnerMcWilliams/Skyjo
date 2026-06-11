#include "./include/skyjo/Game.h"
#include "./include/skyjo/agents/Agent.h"
#include "./include/skyjo/agents/RandomAgent.h"
#include "./include/skyjo/SpeedExperiment.h"

#include <iostream>
#include <cstdio>
#include <map>
#include <memory>
#include <chrono>
#include <fstream>
#include <vector>

#include "include/skyjo/Experiment.h"
#include "include/skyjo/agents/BasicLogic.h"
#include "include/skyjo/agents/MinimaxAgent.h"
#include "include/skyjo/agents/MonteCarloAgent.h"

void RunNormalExperiment(const int num_games, const Game &game, std::vector<std::unique_ptr<Agent>> &agents) {
    using Clock = std::chrono::steady_clock;

    const auto start = Clock::now();

    const ExperimentResult result = RunExperiment(game, agents, num_games);

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

void SpeedExperiment(const int seconds, const int sample_every_ms, const Game &game, std::vector<std::unique_ptr<Agent>> &agents) {
    const SpeedExperimentResult result =
        RunSpeedExperiment(game, agents, seconds, sample_every_ms);

    freopen("./SpeedExperiment_0.txt", "w", stdout);
    std::cout << "Games played: " << result.num_experiments << "\n";
    std::cout << "Elapsed time: " << result.elapsed_ms << " ms\n";
    std::cout << "Average speed: "
              << result.experiments_per_ms
              << " games/ms\n";

    std::cout << "Average speed: "
              << result.experiments_per_ms * 1000.0
              << " games/sec\n\n";

    for (int p = 0; p < game.NumPlayers(); ++p) {
        std::cout << "Agent " << p << ":\n";
        std::cout << "  Wins: " << result.wins[p] << "\n";
        std::cout << "  Average score: " << result.average_scores[p] << "\n\n";
    }
}

int main() {
    const std::map<int, int> distribution = {
        {-2, 5},
        {-1, 10},
        {0, 15},
        {1, 10},
        {2, 10},
        {3, 10},
        {4, 10},
        {5, 10},
        {6, 10},
        {7, 10},
        {8, 10},
        {9, 10},
        {10, 10},
        {11, 10},
        {12, 10}
    };

    const Game game(6, distribution);

    std::vector<std::unique_ptr<Agent>> agents;
    agents.push_back(std::make_unique<MonteCarloAgent>(5000, 2.5, 40));
    agents.push_back(std::make_unique<MonteCarloAgent>(5000, 2.5, 50));

    agents.push_back(std::make_unique<MinimaxAgent>(4));
    agents.push_back(std::make_unique<MinimaxAgent>(4));

    agents.push_back(std::make_unique<RandomAgent>(60));
    agents.push_back(std::make_unique<RandomAgent>(70));

    // TODO: Make all of this run in parallel.

    constexpr double seconds = 100;
    constexpr int sample_every_ms = 10;

    SpeedExperiment(seconds, sample_every_ms, game, agents);

    //RunNormalExperiment(1, game, agents);

    return 0;
}
