#include "./include/skyjo/Game.h"
#include "./include/skyjo/agents/Agent.h"
#include "./include/skyjo/agents/RandomAgent.h"
#include "./include/skyjo/SpeedExperiment.h"
#include "./include/skyjo/Tournament.h"

#include <iostream>
#include <cstdio>
#include <map>
#include <memory>
#include <chrono>
#include <fstream>
#include <iomanip>
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

void TournamentExperiment(
    const Game& game,
    const std::vector<TournamentAgent>& tournament_agents,
    const int rounds,
    const unsigned int seed
    ) {

    std::cout << std::fixed << std::setprecision(2);

    TournamentResults results = ConductTournamentWithThread(game, tournament_agents, rounds, seed, 32);

    for (std::size_t id = 0; id < tournament_agents.size(); ++id) {
        const auto& stats = results.agent_statistics[id];

        std::cout << tournament_agents[id].name << '\n';
        std::cout << "  Elo: " << stats.elo << '\n';
        std::cout << "  Games: " << stats.games_played << '\n';
        std::cout << "  Wins: " << stats.outright_wins << '\n';

        const double average_score =
            stats.games_played == 0
                ? 0.0
                : static_cast<double>(stats.total_score) /
                      static_cast<double>(stats.games_played);

        std::cout << "  Average score: " << average_score << "\n\n";
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

    const Game game(2, distribution);

    std::vector<std::unique_ptr<Agent>> agents;
    agents.push_back(std::make_unique<MonteCarloAgent>(5000, 2.5, 40));
    agents.push_back(std::make_unique<MonteCarloAgent>(5000, 2.5, 50));

    agents.push_back(std::make_unique<MinimaxAgent>(4));
    agents.push_back(std::make_unique<MinimaxAgent>(4));

    agents.push_back(std::make_unique<RandomAgent>(60));
    agents.push_back(std::make_unique<RandomAgent>(70));

    constexpr double seconds = 100;
    constexpr int sample_every_ms = 10;
    constexpr int rounds = 1000;

    const TournamentAgent tournament_agent_minimax_3 {
        .name = "MinimaxAgent_3",
        .factory = [](unsigned int seed) {
            return std::make_unique<MinimaxAgent>(3);
        }
    };

    const TournamentAgent tournament_agent_minimax_4 {
        .name = "MinimaxAgent_4",
        .factory = [](unsigned int seed) {
            return std::make_unique<MinimaxAgent>(4);
        }
    };

    const TournamentAgent tournament_agent_monte_carlo_1000 {
        .name = "MonteCarloAgent",
        .factory = [](unsigned int seed) {
            return std::make_unique<MonteCarloAgent>(5000, 2, seed);
        }
    };

    const TournamentAgent tournament_agent_random {
        .name = "RandomAgent",
        .factory = [](unsigned int seed) {
            return std::make_unique<RandomAgent>(seed);
        }
    };

    const TournamentAgent tournament_agent_logic {
        .name = "BasicLogic",
        .factory = [](unsigned int seed) {
            return std::make_unique<BasicLogic>(3, seed);
        }
    };

    const std::vector tournament_agents{
        tournament_agent_random,
        tournament_agent_logic,
        tournament_agent_minimax_3,
        tournament_agent_monte_carlo_1000
    };

    TournamentExperiment(game, tournament_agents, rounds, 42);

    //SpeedExperiment(seconds, sample_every_ms, game, agents);

    //RunNormalExperiment(1, game, agents);

    return 0;
}
