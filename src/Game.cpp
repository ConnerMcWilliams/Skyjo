//
// Created by Conner on 5/26/2026.
//

#include "../include/skyjo/Game.h"
#include <map>
#include <stdexcept>

Game::Game(int numPlayers, std::map<int, int> distribution) :
    num_players_(numPlayers),
    distribution_(distribution) {
    if (numPlayers < 2 || numPlayers > 8) {
        throw std::invalid_argument("The number of players must be between 2-8.");
    }
}

int Game::NumPlayers() const {
    return num_players_;
}

Action Game::SerializeAction(const SkyjoAction &action) const {
    switch (action.type) {
        case ActionType::DrawFromDeck:
            return 0;

        case ActionType::DrawFromDiscard:
            return 1;

        case ActionType::ReplaceCard:
            if (action.position < 0 || action.position >= 12) {
                throw std::invalid_argument("Invalid ReplaceCard position.");
            }
            return 2 + action.position;

        case ActionType::DiscardDrawnCardAndReveal:
            if (action.position < 0 || action.position >= 4) {
                throw std::invalid_argument(
                    "Invalid DiscardDrawnCardAndReveal position."
                );
            }
            return 14 + action.position;

        case ActionType::RevealInitialCard:
            if (action.position < 0 || action.position >= 2) {
                throw std::invalid_argument("Invalid RevealInitialCard position.");
            }
            return 18 + action.position;

        case ActionType::ChanceCardDraw:
            if (action.position < -2 || action.position >= 12) {
                throw std::invalid_argument("Invalid ChanceCard value.");
            }
            // The two is for the -2 and -1.
            return (20 + 2) + action.position;
        case ActionType::RealDraw:
            return 35;
    }

    throw std::invalid_argument("Unknown action type.");
}

SkyjoAction Game::DeserializeAction(const Action action) const {
    if (action == 0) {
        return SkyjoAction{ActionType::DrawFromDeck, -1};
    }

    if (action == 1) {
        return SkyjoAction{ActionType::DrawFromDiscard, -1};
    }

    if (action >= 2 && action < 14) {
        return SkyjoAction{ActionType::ReplaceCard, action - 2};
    }

    if (action >= 14 && action < 18) {
        return SkyjoAction{
            ActionType::DiscardDrawnCardAndReveal,
            action - 14
        };
    }

    if (action >= 18 && action < 20) {
        return SkyjoAction{
            ActionType::RevealInitialCard,
            action - 18
        };
    }
    if (action >= 20 && action < 35) {
        return SkyjoAction{
        ActionType::ChanceCardDraw,
        action - 22};
    }
    if (action == 35) {
        return SkyjoAction{
        ActionType::RealDraw,
        -1};
    }

    throw std::invalid_argument("Invalid action id.");
}

int Game::NumDistinctActions() {

    //0      = DrawFromDeck
    //1      = DrawFromDiscard
    //2-13   = ReplaceCard at positions 0-11
    //14-16  = DiscardDrawnCardAndReveal at positions 0-3
    //17-18  = RevealInitialCard at positions 0-11
    //19-34  = ChanceCardDraw at values -2-12
    //35     = RealDraw

    return 35;
}

const std::map<int, int>& Game::distribution() const {
    return distribution_;
}
