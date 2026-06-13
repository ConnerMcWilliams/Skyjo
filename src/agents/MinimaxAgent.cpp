//
// Created by Conner on 6/1/2026.
//

#include "../../include/skyjo/agents/MinimaxAgent.h"

MinimaxAgent::MinimaxAgent(const int depth) : depth_(depth) {
}

Action MinimaxAgent::ChooseAction(const State &state) {

    const std::vector<Action> legal_actions = state.LegalActions();
    double best_eval = -std::numeric_limits<double>::infinity();

    double alpha = -std::numeric_limits<double>::infinity();
    double beta = std::numeric_limits<double>::infinity();

    Action best_action = legal_actions.front();
    const int root_player = state.CurrentPlayer();

    for (int i = 0; i < legal_actions.size(); ++i) {
        State child = state;
        child.ApplyAction(legal_actions[i]);

        double eval = Search(child, root_player, depth_ - 1, alpha, beta);

        if (eval > best_eval) {
            best_eval = eval;
            best_action = legal_actions[i];
        }

        alpha = std::max(alpha, best_eval);

    }

    return best_action;
}

double MinimaxAgent::Search(State state, int root_player,
    int current_depth, double alpha, double beta) const {
    if (state.IsTerminal()) {
        return EvalFunction(state, root_player);
    }
    if (current_depth == 0) {
        return EvalFunction(state, root_player);
    }
    if (state.IsChancePhase()) {
        return ChanceSearch(state, root_player, current_depth, alpha, beta);
    }

    const std::vector<Action> legal_actions = state.LegalActions();
    double best_eval = 0;

    if (state.CurrentPlayer() == root_player) {
        best_eval = -std::numeric_limits<double>::infinity();
        for (int i = 0; i<legal_actions.size(); ++i) {

            State child = state;
            child.ApplyAction(legal_actions[i]);

            double cur_eval = Search(child, root_player, current_depth - 1, alpha, beta);

            best_eval = std::max(cur_eval, best_eval);
            alpha = std::max(best_eval, alpha);

            if (beta <= alpha) {
                break;
            }

        }
    } else {
        best_eval = std::numeric_limits<double>::infinity();
        for (int i = 0; i<legal_actions.size(); ++i) {

            State child = state;
            child.ApplyAction(legal_actions[i]);

            double cur_eval = Search(child, root_player, current_depth - 1, alpha, beta);

            best_eval = std::min(cur_eval, best_eval);
            beta = std::min(best_eval, beta);

            if (beta <= alpha) {
                break;
            }

        }
    }

    return best_eval;
}

double MinimaxAgent::ChanceSearch(State state, int root_player,
    int current_depth, double alpha, double beta) const {
    std::array<float, 15> chance_outcomes = state.ChanceOutcome();
    double expected_value = 0;
    for (int val = -2; val <= 12; ++val) {
        State child = state;
        child.ApplyAction(20 + 2 + val);

        expected_value += chance_outcomes[val + 2] * Search(child, root_player, current_depth, alpha, beta);
    }

    return expected_value;
}

double MinimaxAgent::EvalFunction(State state, int root_player) const {
    return -state.Scores()[root_player];
}
