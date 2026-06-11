//
// Created by Conner on 5/30/2026.
//

#include "../include/skyjo/Simulation.h"
#include "../include/skyjo/Game.h"
#include "../include/skyjo/State.h"
#include "../include/skyjo/agents/Agent.h"

#include <stdexcept>

GameResult RunSingleGame(
    const Game& game,
    std::vector<std::unique_ptr<Agent>>& agents
) {
    if (static_cast<int>(agents.size()) != game.NumPlayers()) {
        throw std::invalid_argument("Number of agents must match number of players.");
    }

    State state(game);

    while (!state.IsTerminal()) {
        if (state.IsChancePhase()) {
            Action real_draw = game.SerializeAction(SkyjoAction(ActionType::RealDraw, -1));
            state.ApplyAction(real_draw);
        } else {
            PlayerId current_player = state.CurrentPlayer();

            Action action = agents[current_player]->ChooseAction(state);

            state.ApplyAction(action);
        }

    }

    std::vector<int> scores = state.Scores();

    int winner = 0;
    for (int i = 1; i < static_cast<int>(scores.size()); ++i) {
        if (scores[i] < scores[winner]) {
            winner = i;
        }
    }

    return GameResult{
        .scores = scores,
        .winner = winner
    };
}
