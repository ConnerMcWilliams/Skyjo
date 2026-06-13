//
// Created by Conner on 6/10/2026.
//

#include "../include/skyjo/Tournament.h"

#include <iostream>

void CalculateElo(
    TournamentResults& tournament_results,
    const MatchupMatrix& matchup_matrix
) {

}

void UpdateMatchupMatrix(MatchupMatrix& matchup_matrix,
    const GameResult& result) {

    if (result.agent_ids.size() != result.scores.size()) {
        throw std::invalid_argument(
            "GameResult agent_ids and scores must have the same size"
        );
    }

    for (std::size_t i = 0; i < result.agent_ids.size(); ++i) {

        for (std::size_t j = i + 1; j < result.agent_ids.size(); ++j) {

            const AgentId player_one = result.agent_ids[i];
            const AgentId player_two = result.agent_ids[j];

            const int player_one_score = result.scores[i];
            const int player_two_score = result.scores[j];

            if (player_one_score < player_two_score) {
                ++matchup_matrix[player_one][player_two].wins;
                ++matchup_matrix[player_two][player_one].losses;
            } else if (player_one_score > player_two_score) {
                ++matchup_matrix[player_one][player_two].losses;
                ++matchup_matrix[player_two][player_one].wins;
            } else {
                ++matchup_matrix[player_one][player_two].ties;
                ++matchup_matrix[player_two][player_one].ties;
            }

        }

    }

}

void UpdateTournamentResults(
    TournamentResults& tournament_results,
    const GameResult& result
) {
    if (result.agent_ids.size() != result.scores.size()) {
        throw std::invalid_argument(
            "GameResult agent_ids and scores must have the same size"
        );
    }

    if (result.agent_ids.empty()) {
        return;
    }

    const int best_score = *std::min_element(
        result.scores.begin(),
        result.scores.end()
    );

    const int num_winners = static_cast<int>(
        std::count(
            result.scores.begin(),
            result.scores.end(),
            best_score
        )
    );

    for (std::size_t i = 0; i < result.agent_ids.size(); ++i) {
        const AgentId agent_id = result.agent_ids[i];

        AgentTournamentStatistics& statistics =
            tournament_results.agent_statistics[agent_id];

        ++statistics.games_played;
        statistics.total_score += result.scores[i];

        if (result.scores[i] == best_score && num_winners == 1) {
            ++statistics.outright_wins;
        }
    }
}

TournamentResults ConductTournament(
    const Game &game,
    const std::vector<TournamentAgent> &agents,
    std::uint64_t rounds,
    unsigned int seed
    ) {

    const std::size_t num_players =
        static_cast<std::size_t>(game.NumPlayers());

    if (agents.size() < num_players) {
        throw std::invalid_argument(
            "Not enough tournament agents for the game's player count"
        );
    }

    const std::size_t num_agents = agents.size();

    TournamentResults tournament_results{
        .agent_statistics =
            std::vector<AgentTournamentStatistics>(num_agents),

        .matchup_matrix = MatchupMatrix(
            num_agents,
            std::vector<PairwiseResult>(num_agents)
        )
    };

    std::mt19937 rng(seed);

    std::vector<AgentId> available_agent_ids(num_agents);
    std::iota(
        available_agent_ids.begin(),
        available_agent_ids.end(),
        AgentId{0}
    );

    for (std::uint64_t round = 0; round < rounds; ++round) {
        std::shuffle(
            available_agent_ids.begin(),
            available_agent_ids.end(),
            rng
        );

        GameResult result;
        result.seed = rng();

        result.agent_ids.assign(
            available_agent_ids.begin(),
            available_agent_ids.begin() +
                static_cast<std::ptrdiff_t>(num_players)
        );

        std::vector<std::unique_ptr<Agent>> game_agents;
        game_agents.reserve(num_players);

        for (std::size_t seat = 0; seat < num_players; ++seat) {
            const AgentId agent_id = result.agent_ids[seat];

            game_agents.push_back(
                agents[agent_id].factory(rng())
            );
        }

        State state(game);

        while (!state.IsTerminal()) {
            if (state.IsChancePhase()) {
                const Action real_draw = game.SerializeAction(
                    SkyjoAction(ActionType::RealDraw, -1)
                );

                state.ApplyAction(real_draw);

                continue;
            }

            const PlayerId current_player = state.CurrentPlayer();

            const Action action =
                game_agents[current_player]->ChooseAction(state);

            state.ApplyAction(action);
        }

        result.scores = state.Scores();

        UpdateMatchupMatrix(
            tournament_results.matchup_matrix,
            result
        );

        UpdateTournamentResults(
            tournament_results,
            result
        );
    }
    return tournament_results;
}
