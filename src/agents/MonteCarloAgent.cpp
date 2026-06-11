//
// Created by Conner on 6/8/2026.
//

#include "../../include/skyjo/agents/MonteCarloAgent.h"
#include <random>
#include <memory>
#include <cmath>
#include <limits>
#include <algorithm>

MonteCarloAgent::MonteCarloAgent(int num_simulations, double exploration_constant, unsigned int seed) :
num_simulations_(num_simulations), rng_(seed), exploration_constant_(exploration_constant) {
}

Action MonteCarloAgent::ChooseAction(const State &state) {
    PlayerId root_player = state.CurrentPlayer();

    MCTSNode root(state, nullptr, -1);

    for (int i = 0; i < num_simulations_; ++i) {

        MCTSNode* node = Select(&root, root_player);

        double cur = Simulate(node->state, root_player);

        BackPropagate(node, cur);

    }

    MCTSNode* best_child = root.child_nodes.front().get();

    for (const auto& child: root.child_nodes) {
        if (best_child->visits < child->visits) {
            best_child = child.get();
        }
    }

    return best_child->action_from_parent;

}

double MonteCarloAgent::NodeEval(MCTSNode *node) {
    if (node->visits == 0) {
        return std::numeric_limits<double>::infinity();
    }

    int parent_visits = std::max(1, node->parent->visits);

    double average = node->value / node->visits;

    double bound = std::log(parent_visits) / node->visits;

    return average + exploration_constant_ * std::sqrt(bound);
}

MCTSNode* MonteCarloAgent::Select(MCTSNode *node, PlayerId root_player) {
    while (!node->state.IsTerminal()) {
        if (!node->actions_not_taken.empty()) {
            return Expand(node, root_player);
        }

        if (node->child_nodes.empty()) {
            return node;
        }

        MCTSNode* best_child = node->child_nodes.front().get();
        double best_eval = NodeEval(best_child);

        for (const auto& child : node->child_nodes) {

            double current_eval = NodeEval(child.get());

            if (current_eval > best_eval) {
                best_child = child.get();
                best_eval = current_eval;
            }
        }

        node = best_child;
    }


    return node;
}

void MonteCarloAgent::ResolveNonRootAndChance(State& child_state, PlayerId root_player) {
    while (!child_state.IsTerminal() &&
        (child_state.IsChancePhase() || child_state.CurrentPlayer() != root_player)) {
        if (child_state.IsChancePhase()) {
            const int sample = child_state.SampleOutcome(rng_);
            child_state.ApplyAction(sample + 22);
        } else {
            const auto legal_actions = child_state.LegalActions();

            std::uniform_int_distribution<std::size_t> dist(
                0,
                legal_actions.size() - 1
            );

            child_state.ApplyAction(legal_actions[dist(rng_)]);
        }

    }
}

MCTSNode* MonteCarloAgent::Expand(MCTSNode *node, const PlayerId root_player) {
    Action new_action = node->actions_not_taken.back();
    node->actions_not_taken.pop_back();

    State child_state = node->state;
    child_state.ApplyAction(new_action);

    ResolveNonRootAndChance(child_state, root_player);

    node->child_nodes.push_back(
        std::make_unique<MCTSNode>(
            child_state,
            node,
            new_action
        )
    );

    return node->child_nodes.back().get();
}

Action MonteCarloAgent::RolloutAction(const State& state, int threshold) {
    const auto legal_actions = state.LegalActions();

    Phase current_phase = state.CurrentPhase();
    switch (current_phase) {
        case (Phase::ChooseDrawSource): {
            const Card top_discard = state.TopDiscardCard();

            if (top_discard.value() < threshold) {
                const Action draw_discard = state.GameInstance().SerializeAction(
                    SkyjoAction{ActionType::DrawFromDiscard, -1}
                );
                return draw_discard;
            }
            const Action draw_deck = state.GameInstance().SerializeAction(
                    SkyjoAction{ActionType::DrawFromDeck, -1}
                );
            return draw_deck;
        }
        case (Phase::ChooseReplaceOrDiscard): {
            bool keep = state.DrawnCard().value() < threshold;

            for (Action action : legal_actions) {
                SkyjoAction decoded = state.GameInstance().DeserializeAction(action);

                if (keep && decoded.type == ActionType::ReplaceCard) {
                    return action;
                }

                if (!keep && decoded.type == ActionType::DiscardDrawnCardAndReveal) {
                    return action;
                }
            }

            return legal_actions[0];

        }
        case (Phase::InitialReveal): {
            return state.LegalActions()[0];
        }

    }
    throw std::invalid_argument("Wrong state for rollout action.");
}

Action MonteCarloAgent::RandomAction(const State& state) {
    const auto legal_actions = state.LegalActions();

    std::uniform_int_distribution<std::size_t> dist(
        0,
        legal_actions.size() - 1
    );

    return legal_actions[dist(rng_)];
}

double MonteCarloAgent::EvaluateTerminal(State state, PlayerId root_player) {
    std::vector<int> scores = state.Scores();
    int opponent_cum_scores = 0;

    for (int i = 0; i < scores.size(); ++i) {
        if (i != root_player) {
            opponent_cum_scores += scores[i];
        }
    }

    double opponent_avg =
    static_cast<double>(opponent_cum_scores) / static_cast<double>(scores.size() - 1);

    return opponent_avg - scores[root_player];
}

double MonteCarloAgent::Simulate(State state, PlayerId root_player) {
    while (!state.IsTerminal()) {
        if (state.IsChancePhase()) {
            const int sample = state.SampleOutcome(rng_);
            state.ApplyAction(sample + 22);
        } else {
            state.ApplyAction(RolloutAction(state, 4));
        }
    }
    return EvaluateTerminal(state, root_player);
}

void MonteCarloAgent::BackPropagate(MCTSNode *node, double result) {
    while (node != nullptr) {
        node->visits += 1;
        node->value += result;
        node = node->parent;
    }
}
