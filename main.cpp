#include "./include/skyjo/Game.h"
#include "./include/agents/Agent.h"
#include "./include/agents/RandomAgent.h"
#include "./include/experiments/SpeedExperiment.h"
#include "./include/experiments/Tournament.h"
#include "include/experiments/Experiment.h"

#include <map>
#include <memory>
#include <chrono>
#include <vector>

#include "include/agents/BasicLogic.h"
#include "include/agents/MinimaxAgent.h"
#include "include/agents/MonteCarloAgent.h"

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

int main(int argc, char* argv[]) {

    bool speed_experiment = false;
    bool basic_experiment = false;
    int iterations = 0;
    bool tournament_experiment = false;
    int num_players = 0;

    for (int i = 0; i < argc; i++) {
        std::string cur = argv[i];
        if (cur == "-speed") {
            speed_experiment = true;
        } else if (cur == "-basic") {
            basic_experiment = true;
            i++;

            if (i > argc) {
                throw std::invalid_argument("Basic experiment did not specify amount of iterations.");
            }

            cur = argv[i];
            char* endptr;
            long val = strtol(cur.c_str(), &endptr, 10);

            if (endptr == cur.c_str()) {
                throw std::invalid_argument("No digits in specified iterations.");
            }

            iterations = val;

        } else if (cur == "-tournament") {
            tournament_experiment = true;

            i++;

            if (i > argc) {
                throw std::invalid_argument("Basic experiment did not specify amount of iterations.");
            }

            cur = argv[i];
            char* endptr;
            long val = strtol(cur.c_str(), &endptr, 10);

            if (endptr == cur.c_str()) {
                throw std::invalid_argument("No digits in specified iterations.");
            }
            num_players = val;
        }
    }

    std::vector<std::unique_ptr<Agent>> agents;
    agents.push_back(std::make_unique<MonteCarloAgent>(500, 2.5, 40));

    agents.push_back(std::make_unique<MinimaxAgent>(3));
    agents.push_back(std::make_unique<MinimaxAgent>(3));

    constexpr double seconds = 100;
    constexpr int sample_every_ms = 10;
    constexpr int rounds = 100;

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
            return std::make_unique<MonteCarloAgent>(1000, 2, seed);
        }
    };

    const TournamentAgent tournament_agent_monte_carlo_5000 {
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

    const TournamentAgent tournament_agent_logic_3 {
        .name = "BasicLogic",
        .factory = [](unsigned int seed) {
            return std::make_unique<BasicLogic>(3, seed);
        }
    };

    const TournamentAgent tournament_agent_logic_4 {
        .name = "BasicLogic",
        .factory = [](unsigned int seed) {
            return std::make_unique<BasicLogic>(4, seed);
        }
    };

    const std::vector tournament_agents{
        tournament_agent_random,
        tournament_agent_logic_3,
        tournament_agent_logic_4,
        tournament_agent_minimax_3,
        tournament_agent_minimax_4,
        tournament_agent_monte_carlo_1000,
        tournament_agent_monte_carlo_5000
    };



    if (tournament_experiment) {
        const Game game(num_players, distribution);
        TournamentExperiment(game, tournament_agents, rounds, 42);
    }
    if (speed_experiment) {
        const Game game(agents.size(), distribution);
        RunSpeedExperiment(seconds, sample_every_ms, game, agents);
    }
    if (basic_experiment) {
        const Game game(agents.size(), distribution);
        RunBasicExperiment(iterations, game, agents);
    }

    return 0;
}
