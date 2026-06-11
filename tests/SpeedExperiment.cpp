//
// Created by Conner on 6/1/2026.
//

#include "../include/skyjo/SpeedExperiment.h"
#include "../include/skyjo/Simulation.h"


#include <chrono>
#include <vector>
#include <memory>

SpeedExperimentResult RunSpeedExperiment(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents,
    double seconds,
    int sample_every_ms
) {
    int num_players = game.NumPlayers();

    SpeedExperimentResult result;
    result.num_experiments = 0;
    result.wins.assign(num_players, 0);
    result.total_scores.assign(num_players, 0.0);
    result.average_scores.assign(num_players, 0.0);

    using Clock = std::chrono::steady_clock;

    auto start = Clock::now();
    auto last_sample_time = start;
    auto next_sample_time = start + std::chrono::milliseconds(sample_every_ms);

    int experiments_at_last_sample = 0;

    while (std::chrono::duration<double>(Clock::now() - start).count() < seconds) {
        GameResult game_result = RunSingleGame(game, agents);

        result.num_experiments++;

        result.wins.at(game_result.winner)++;

        for (int p = 0; p < num_players; ++p) {
            result.total_scores.at(p) += game_result.scores.at(p);
        }

        auto now = Clock::now();

        if (now >= next_sample_time) {
            double elapsed_ms =
                std::chrono::duration<double, std::milli>(now - start).count();

            double interval_ms =
                std::chrono::duration<double, std::milli>(now - last_sample_time).count();

            int interval_experiments =
                result.num_experiments - experiments_at_last_sample;

            SpeedSample sample;
            sample.elapsed_ms = elapsed_ms;
            sample.total_experiments = result.num_experiments;

            if (elapsed_ms > 0.0) {
                sample.cumulative_experiments_per_ms =
                    result.num_experiments / elapsed_ms;
            }

            if (interval_ms > 0.0) {
                sample.recent_experiments_per_ms =
                    interval_experiments / interval_ms;
            }

            result.speed_samples.push_back(sample);

            last_sample_time = now;
            experiments_at_last_sample = result.num_experiments;

            while (next_sample_time <= now) {
                next_sample_time += std::chrono::milliseconds(sample_every_ms);
            }
        }
    }

    auto end = Clock::now();

    result.elapsed_ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    if (result.elapsed_ms > 0.0) {
        result.experiments_per_ms =
            result.num_experiments / result.elapsed_ms;
    }

    if (result.num_experiments > 0) {
        for (int p = 0; p < num_players; ++p) {
            result.average_scores[p] =
                result.total_scores[p] / static_cast<double>(result.num_experiments);
        }
    }

    return result;
}
