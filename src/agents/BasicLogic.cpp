//
// Created by Conner on 6/1/2026.
//

#include "../../include/skyjo/agents/BasicLogic.h"
#include "../../include/skyjo/State.h"

#include <random>
#include <stdexcept>
#include <vector>

BasicLogic::BasicLogic(double card_avg, unsigned int seed)
    : card_avg_(card_avg), rng_(seed) {
}

Action BasicLogic::ChooseAction(const State& state) {
    const Phase curr_phase = state.CurrentPhase();

    const std::vector<Action> legal_actions = state.LegalActions();

    if (legal_actions.empty()) {
        throw std::runtime_error("BasicLogic::ChooseAction(): no legal actions");
    }

    std::uniform_int_distribution<int> dist_actions(
        0,
        static_cast<int>(legal_actions.size()) - 1
    );

    // Safe fallback: random legal action.
    Action action = legal_actions[dist_actions(rng_)];

    switch (curr_phase) {
        case Phase::ChooseDrawSource: {
            const Card top_discard = state.TopDiscardCard();

            if (top_discard.value() < card_avg_) {
                const Action draw_discard = state.GameInstance().SerializeAction(
                    SkyjoAction{ActionType::DrawFromDiscard, -1}
                );

                for (Action legal_action : legal_actions) {
                    if (legal_action == draw_discard) {
                        return draw_discard;
                    }
                }
            }

            const Action draw_deck = state.GameInstance().SerializeAction(
                SkyjoAction{ActionType::DrawFromDeck, -1}
            );

            for (Action legal_action : legal_actions) {
                if (legal_action == draw_deck) {
                    return draw_deck;
                }
            }

            return action;
        }

        case Phase::ChooseReplaceOrDiscard: {
            if (state.DrawnCard().value() < card_avg_) {
                std::vector<Action> replace_actions;

                for (Action legal_action : legal_actions) {
                    SkyjoAction decoded =
                        state.GameInstance().DeserializeAction(legal_action);

                    if (decoded.type == ActionType::ReplaceCard) {
                        replace_actions.push_back(legal_action);
                    }
                }

                if (!replace_actions.empty()) {
                    std::uniform_int_distribution<int> dist_replace(
                        0,
                        static_cast<int>(replace_actions.size()) - 1
                    );

                    return replace_actions[dist_replace(rng_)];
                }
            }

            return action;
        }

        case Phase::InitialReveal: {
            return action;
        }

        case Phase::Terminal: {
            throw std::runtime_error("BasicLogic::ChooseAction(): terminal state");
        }
    }

    return action;
}