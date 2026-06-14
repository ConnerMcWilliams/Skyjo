//
// Created by Conner on 6/10/2026.
//

#ifndef SKYJO_TOURNAMENT_H
#define SKYJO_TOURNAMENT_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Game.h"
#include "agents/Agent.h"

using AgentId = std::uint16_t;

struct TournamentAgent {
    std::string name;

    std::function<std::unique_ptr<Agent>(unsigned int seed)> factory;
};

struct AgentTournamentStatistics {
    std::uint64_t games_played = 0;
    std::uint64_t outright_wins = 0;

    std::int64_t total_score = 0;

    double elo = 1500.0;
};

struct PairwiseResult {
    std::uint64_t wins = 0;
    std::uint64_t losses = 0;
    std::uint64_t ties = 0;
};

using MatchupMatrix =
    std::vector<std::vector<PairwiseResult>>;

struct TournamentResults {
    std::vector<AgentTournamentStatistics> agent_statistics;

    MatchupMatrix matchup_matrix;
};

struct GameResult {
    std::vector<AgentId> agent_ids;
    std::vector<int> scores;

    std::uint64_t seed = 0;
};

void CalculateElo(
    TournamentResults& tournament_results,
    const MatchupMatrix& matchup_matrix
);

void UpdateMatchupMatrix(
    MatchupMatrix& matchup_matrix,
    const GameResult& result
);

void UpdateTournamentResults(
    TournamentResults& tournament_results,
    const GameResult& result
);

[[nodiscard]] GameResult RunOneGame(
    const Game& game,
    unsigned int seed,
    std::vector<AgentId> available_agent_ids,
    std::size_t num_players,
    const std::vector<TournamentAgent> &agents
    );

[[nodiscard]] TournamentResults ConductTournament(
    const Game& game,
    const std::vector<TournamentAgent>& agents,
    std::uint64_t rounds,
    unsigned int seed
);

[[nodiscard]] TournamentResults ConductTournamentWithThread(
    const Game& game,
    const std::vector<TournamentAgent>& agents,
    std::uint64_t rounds,
    unsigned int seed,
    unsigned int threads
);

#endif // SKYJO_TOURNAMENT_H