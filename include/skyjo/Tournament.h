//
// Created by Conner on 6/10/2026.
//

#ifndef SKYJO_TOURNAMENT_H
#define SKYJO_TOURNAMENT_H
#include <functional>
#include <memory>
#include <vector>

#include "agents/Agent.h"

struct TournamentAgent {
    std::string name;
    std::function<std::unique_ptr<Agent>(unsigned int seed)> factory;
    double elo = 1000.0;
    int games_played = 0;
    int wins = 0;
    double total_score = 0.0;
};

struct TournamentResults {
    std::map<std::string, int> elo;
    std::map<std::string, int> wins;
    std::map<std::string, int> total_score;
    std::map<std::string, int> games_played;
};

void CalculateElo(std::vector<std::unique_ptr<TournamentAgent>> tournament_agents);

TournamentResults ConductTournament(Game game, std::vector<Agent> agents, int rounds);

#endif //SKYJO_TOURNAMENT_H
