//
// Created by Conner on 6/10/2026.
//

#include "../include/skyjo/Tournament.h"

#include <iostream>

#include "../include/skyjo/Tournament.h"

#include <thread>
#include <algorithm>
#include <cmath>
#include <future>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

void CalculateElo(
    TournamentResults &tournament_results,
    const MatchupMatrix &matchup_matrix
) {
    constexpr double initial_elo = 1500.0;
    constexpr double elo_scale = 400.0;

    constexpr int max_iterations = 10'000;
    constexpr double convergence_tolerance = 1e-10;

    const std::size_t num_agents = matchup_matrix.size();

    if (tournament_results.agent_statistics.size() != num_agents) {
        throw std::invalid_argument(
            "Agent statistics and matchup matrix must have matching sizes"
        );
    }

    for (const auto &row: matchup_matrix) {
        if (row.size() != num_agents) {
            throw std::invalid_argument(
                "Matchup matrix must be square"
            );
        }
    }

    if (num_agents == 0) {
        return;
    }

    /*
     * Bradley-Terry strengths.
     *
     * P(i beats j) = strength[i] /
     *                  (strength[i] + strength[j])
     *
     * All agents begin with equal strength.
     */
    std::vector<double> strengths(num_agents, 1.0);
    std::vector<double> effective_wins(num_agents, 0.0);
    std::vector<double> denominator(num_agents, 0.0);
    std::vector<bool> has_games(num_agents, false);

    /*
     * A tie contributes half a win to each agent.
     *
     * Only inspect matrix[i][j] where i < j because the opposite
     * entry contains the same matchup from the other perspective.
     */
    for (std::size_t i = 0; i < num_agents; ++i) {
        for (std::size_t j = i + 1; j < num_agents; ++j) {
            const PairwiseResult &result = matchup_matrix[i][j];

            const std::uint64_t total_games =
                    result.wins + result.losses + result.ties;

            if (total_games == 0) {
                continue;
            }

            has_games[i] = true;
            has_games[j] = true;

            effective_wins[i] +=
                    static_cast<double>(result.wins) +
                    0.5 * static_cast<double>(result.ties);

            effective_wins[j] +=
                    static_cast<double>(result.losses) +
                    0.5 * static_cast<double>(result.ties);
        }
    }

    /*
     * Small regularization prevents undefeated or winless agents from
     * receiving infinite ratings.
     *
     * This is equivalent to adding a tiny amount of prior evidence that
     * each participating agent has both won and lost.
     */
    constexpr double prior_wins = 0.5;

    for (std::size_t i = 0; i < num_agents; ++i) {
        if (has_games[i]) {
            effective_wins[i] += prior_wins;
        }
    }

    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        std::fill(
            denominator.begin(),
            denominator.end(),
            0.0
        );

        for (std::size_t i = 0; i < num_agents; ++i) {
            for (std::size_t j = i + 1; j < num_agents; ++j) {
                const PairwiseResult &result = matchup_matrix[i][j];

                const double total_games = static_cast<double>(
                    result.wins + result.losses + result.ties
                );

                if (total_games == 0.0) {
                    continue;
                }

                const double strength_sum =
                        strengths[i] + strengths[j];

                denominator[i] += total_games / strength_sum;
                denominator[j] += total_games / strength_sum;
            }
        }

        /*
         * Match the prior win added above with a small denominator term.
         * This pulls extreme estimates slightly toward strength 1.0.
         */
        for (std::size_t i = 0; i < num_agents; ++i) {
            if (has_games[i]) {
                denominator[i] += prior_wins;
            }
        }

        std::vector<double> updated_strengths = strengths;

        double maximum_change = 0.0;

        for (std::size_t i = 0; i < num_agents; ++i) {
            if (!has_games[i] || denominator[i] == 0.0) {
                updated_strengths[i] = 1.0;
                continue;
            }

            updated_strengths[i] =
                    effective_wins[i] / denominator[i];

            updated_strengths[i] = std::max(
                updated_strengths[i],
                std::numeric_limits<double>::min()
            );
        }

        /*
         * Bradley-Terry strengths are only identifiable up to a common
         * multiplier. Normalize their geometric mean to 1.0 so the
         * corresponding average Elo is 1500.
         */
        double sum_log_strengths = 0.0;
        std::size_t participating_agents = 0;

        for (std::size_t i = 0; i < num_agents; ++i) {
            if (!has_games[i]) {
                continue;
            }

            sum_log_strengths += std::log(updated_strengths[i]);
            ++participating_agents;
        }

        if (participating_agents > 0) {
            const double geometric_mean = std::exp(
                sum_log_strengths /
                static_cast<double>(participating_agents)
            );

            for (std::size_t i = 0; i < num_agents; ++i) {
                if (has_games[i]) {
                    updated_strengths[i] /= geometric_mean;
                }
            }
        }

        for (std::size_t i = 0; i < num_agents; ++i) {
            maximum_change = std::max(
                maximum_change,
                std::abs(updated_strengths[i] - strengths[i])
            );
        }

        strengths = std::move(updated_strengths);

        if (maximum_change < convergence_tolerance) {
            break;
        }
    }

    /*
     * Convert Bradley-Terry strength to Elo:
     *
     * rating_i = 1500 + 400 log10(strength_i)
     */
    for (std::size_t i = 0; i < num_agents; ++i) {
        AgentTournamentStatistics &statistics =
                tournament_results.agent_statistics[i];

        if (!has_games[i]) {
            statistics.elo = initial_elo;
            continue;
        }

        statistics.elo =
                initial_elo +
                elo_scale * std::log10(strengths[i]);
    }
}

void UpdateMatchupMatrix(MatchupMatrix &matchup_matrix,
                         const GameResult &result) {
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
    TournamentResults &tournament_results,
    const GameResult &result
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

        AgentTournamentStatistics &statistics =
                tournament_results.agent_statistics[agent_id];

        ++statistics.games_played;
        statistics.total_score += result.scores[i];

        if (result.scores[i] == best_score && num_winners == 1) {
            ++statistics.outright_wins;
        }
    }
}

GameResult RunOneGame(
    const Game &game,
    unsigned int seed,
    std::vector<AgentId> available_agent_ids,
    const std::size_t num_players,
    const std::vector<TournamentAgent> &agents
) {
    std::mt19937 rng(seed);
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

    std::vector<std::unique_ptr<Agent> > game_agents;
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

    return result;
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

    std::vector<AgentId> available_agent_ids(num_agents);
    std::iota(
        available_agent_ids.begin(),
        available_agent_ids.end(),
        AgentId{0}
    );

    for (std::uint64_t round = 0; round < rounds; ++round) {

        GameResult result = RunOneGame(
            game,
            seed,
            available_agent_ids,
            num_players,
            agents
        );

        UpdateMatchupMatrix(
            tournament_results.matchup_matrix,
            result
        );

        UpdateTournamentResults(
            tournament_results,
            result
        );
    }
    CalculateElo(
        tournament_results,
        tournament_results.matchup_matrix
    );

    return tournament_results;
}

TournamentResults ConductTournamentWithThread(
    const Game &game,
    const std::vector<TournamentAgent> &agents,
    std::uint64_t rounds,
    unsigned int seed,
    unsigned int threads
) {
    const std::size_t num_players =
            static_cast<std::size_t>(game.NumPlayers());

    if (agents.size() < num_players) {
        throw std::invalid_argument(
            "Not enough tournament agents for the game's player count"
        );
    }

    std::mt19937 rng(seed);

    const std::size_t num_agents = agents.size();

    TournamentResults tournament_results{
        .agent_statistics =
        std::vector<AgentTournamentStatistics>(num_agents),

        .matchup_matrix = MatchupMatrix(
            num_agents,
            std::vector<PairwiseResult>(num_agents)
        )
    };

    std::vector<AgentId> available_agent_ids(num_agents);
    std::iota(
        available_agent_ids.begin(),
        available_agent_ids.end(),
        AgentId{0}
    );

    for (std::uint64_t round = 0; round < rounds / threads; ++round) {

        std::vector<std::future<GameResult>> results;
        results.reserve(threads);

        for (int i = 0; i < threads; ++i) {
            const unsigned int game_seed = rng();
            results.emplace_back(std::async(
                    std::launch::async,
                    RunOneGame,
                    std::cref(game),
                    game_seed,
                    available_agent_ids,
                    num_players,
                    std::cref(agents)
                )
            );
        }

        for (std::future<GameResult>& future : results) {
            GameResult result = future.get();
            UpdateMatchupMatrix(
                tournament_results.matchup_matrix,
                result
            );

            UpdateTournamentResults(
                tournament_results,
                result
            );
        }

    }
    CalculateElo(
        tournament_results,
        tournament_results.matchup_matrix
    );

    return tournament_results;
}
