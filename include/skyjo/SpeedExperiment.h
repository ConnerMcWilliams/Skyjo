//
// Created by conne on 6/1/2026.
//

#ifndef SKYJO_SPEEDEXPERIMENT_H
#define SKYJO_SPEEDEXPERIMENT_H

#include "Game.h"
#include "./agents/Agent.h"

#include <memory>
#include <vector>

struct SpeedSample {
    double elapsed_ms = 0.0;
    int total_experiments = 0;

    // Total experiments / total elapsed milliseconds
    double cumulative_experiments_per_ms = 0.0;

    // Experiments since last sample / milliseconds since last sample
    double recent_experiments_per_ms = 0.0;
};

struct SpeedExperimentResult {
    int num_experiments = 0;

    std::vector<int> wins;
    std::vector<double> total_scores;
    std::vector<double> average_scores;

    double elapsed_ms = 0.0;
    double experiments_per_ms = 0.0;

    std::vector<SpeedSample> speed_samples;
};

SpeedExperimentResult RunSpeedExperiment(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents,
    double seconds,
    int sample_every_ms = 100
);

#endif //SKYJO_SPEEDEXPERIMENT_H
