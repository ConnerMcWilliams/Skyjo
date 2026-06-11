//
// Created by Conner on 5/28/2026.
//

#include "../include/skyjo/State.h"
#include "../include/skyjo/Action.h"
#include "../include/skyjo/Player.h"
#include "../include/skyjo/Card.h"
#include <memory_resource>
#include <stdexcept>

State::State(const Game& game)
    : game_(game),
      current_seen_dist_(game.distribution()),
      total_unseen_cards_(150),
      deck_(150, game.distribution()),
      phase_(Phase::InitialReveal),
      drawn_card_(std::nullopt),
      player_who_went_out_(std::nullopt),
      initial_turns_remaining_(game.NumPlayers() * 2) {
    DealInitialCards();
}

bool State::IsChancePhase() const {
    return phase_ == Phase::ChanceDrawFromDeck;
}

std::vector<Action> State::LegalActions() const {
    std::vector<Action> legal_actions;
    legal_actions.reserve(22);

    switch (phase_) {
        case Phase::ChooseDrawSource: {
            // There are two draw sources to choose from:
            // 1. The Deck
            // 2. The Discard

            legal_actions.push_back(game_.SerializeAction(
                SkyjoAction{ActionType::DrawFromDeck, -1}
            ));
            // Only legal to draw from the discard pile if it has a card.
            if (!deck_.discardPileEmpty()) {
                legal_actions.push_back(game_.SerializeAction(
                    SkyjoAction{ActionType::DrawFromDiscard, -1}
                ));
            }
            break;
        }

        case Phase::ChooseReplaceOrDiscard: {
            // Multiple choices:
            // 1. to replace one of the FACE UP cards
            // 2. to discard the revealed card and reveal a FACE UP card

            uint16_t replace = players_[current_player_].ValidReplaceMask();
            uint16_t column = players_[current_player_].GetColumnMask();

            column = column << 3;
            if (column & 1 << 0) {
                legal_actions.push_back(game_.SerializeAction(
                    SkyjoAction{ActionType::DiscardDrawnCardAndReveal, 0}
                ));
            }
            for (int i = 1; i < 4; ++i) {
                column = column << 4;
                if (column & 1 << 0) {
                    legal_actions.push_back(game_.SerializeAction(
                        SkyjoAction{ActionType::DiscardDrawnCardAndReveal, i}
                    ));
                }
            }

            while (replace != 0) {
                const int pos = std::countr_zero(replace);

                legal_actions.push_back(game_.SerializeAction(
                    SkyjoAction{ActionType::ReplaceCard, pos}
                ));

                replace &= replace - 1;
            }
            break;
        }

        case Phase::InitialReveal: {
            legal_actions.push_back(game_.SerializeAction(
                    SkyjoAction{ActionType::RevealInitialCard, 0}
                    ));
            legal_actions.push_back(game_.SerializeAction(
                    SkyjoAction{ActionType::RevealInitialCard, 1}
                    ));
            break;
        }

        case Phase::Terminal:
            break;
    }

    return legal_actions;
}

void State::ApplyAction(const Action action) {
    auto [type, position] = game_.DeserializeAction(action);
    const ActionType choice = type;
    const int pos = position;

    switch (phase_) {
        case Phase::ChanceDrawFromDeck: {
            if (type == ActionType::RealDraw) {
                drawn_card_ = deck_.drawCard(DrawLocation::DrawPile);
                current_seen_dist_[drawn_card_.value().value()]--;
                total_unseen_cards_--;

                phase_ = Phase::ChooseReplaceOrDiscard;
            } else {
                Card chance_card(position);
                drawn_card_ = chance_card;
                current_seen_dist_[position]--;
                total_unseen_cards_--;

                phase_ = Phase::ChooseReplaceOrDiscard;
            }

            break;
        }
        case Phase::ChooseDrawSource: {
            switch (choice) {
                case ActionType::DrawFromDeck:
                    phase_ = Phase::ChanceDrawFromDeck;
                    break;

                case ActionType::DrawFromDiscard:
                    drawn_card_ = deck_.drawCard(DrawLocation::DiscardPile);
                    phase_ = Phase::ChooseReplaceOrDiscard;
                    break;

                default:
                    throw std::runtime_error("Invalid action during ChooseDrawSource");
            }
            break;
        }

        case Phase::ChooseReplaceOrDiscard: {
            if (!drawn_card_.has_value()) {
                throw std::runtime_error("No drawn card available");
            }

            switch (choice) {
                case ActionType::ReplaceCard: {
                    std::stack<Card> discarded_cards =
                        players_[current_player_].ReplaceCard(pos, *drawn_card_);

                    while (!discarded_cards.empty()) {
                        Card current_card = discarded_cards.top();
                        discarded_cards.pop();

                        current_seen_dist_[current_card.value()]--;
                        total_unseen_cards_--;
                        deck_.discardCard(current_card);
                    }

                    drawn_card_ = std::nullopt;

                    CheckForEndOfRound();
                    if (!IsTerminal()) {
                        AdvanceTurn();
                        phase_ = Phase::ChooseDrawSource;
                    }
                    break;
                }

                case ActionType::DiscardDrawnCardAndReveal: {
                    deck_.discardCard(*drawn_card_);
                    drawn_card_ = std::nullopt;

                    const RevealResult result = players_[current_player_].RevealCard(pos);

                    current_seen_dist_[result.revealed_card.value()]--;
                    total_unseen_cards_--;
                    if (result.ClearedColumn()) {
                        for (int i = 0; i < result.num_discarded; ++i) {
                            deck_.discardCard(result.discarded_cards[i].value());
                        }
                    }

                    CheckForEndOfRound();
                    if (!IsTerminal()) {
                        AdvanceTurn();
                        phase_ = Phase::ChooseDrawSource;
                    }
                    break;
                }

                default:
                    throw std::runtime_error("Invalid action during ChooseReplaceOrDiscard");
            }
            break;
        }

        case Phase::InitialReveal: {
            if (choice != ActionType::RevealInitialCard) {
                throw std::runtime_error("Invalid action during InitialReveal");
            }

            const RevealResult result = players_[current_player_].RevealCard(pos);

            current_seen_dist_[result.revealed_card.value()]--;
            total_unseen_cards_--;

            initial_turns_remaining_--;
            if (initial_turns_remaining_ == 0) {
                AdvanceTurn();
            } else {
                current_player_ = (current_player_ + 1) % game_.NumPlayers();
            }
            break;
        }

        case Phase::Terminal:
            throw std::runtime_error("Cannot apply action to terminal state");
    }
}

bool State::IsTerminal() const {
    return phase_ == Phase::Terminal;
}

Card State::TopDiscardCard() const {
    const Card top_discard_card = deck_.discardTop();
    return top_discard_card;
}

std::vector<int> State::Scores() const {
    std::vector<int> score;
    const int num_players = game_.NumPlayers();
    score.reserve(num_players);

    for (int pos = 0; pos < num_players; ++pos) {
        score.push_back(players_[pos].Score());
    }

    return score;
}

std::array<CardSlot, 12> State::GetCurrentPlayerHand() const {
    return players_[current_player_].GetHand();
}

void State::AdvanceTurn() {
    phase_ = Phase::ChooseDrawSource;
    current_player_ = (current_player_ + 1) % game_.NumPlayers();
}

Card State::DrawnCard() const {
    if (phase_ != Phase::ChooseReplaceOrDiscard) {
        throw std::runtime_error("Invalid action during DrawnCard");
    }

    if (!drawn_card_.has_value()) {
        throw std::runtime_error("DrawnCard() called but drawn_card_ has no value");
    }

    return drawn_card_.value();
}

void State::CheckForEndOfRound() {
    if (phase_ == Phase::Terminal) {
        return;
    }

    if (!player_who_went_out_.has_value()) {
        if (players_[current_player_].AllCardsFaceUp()) {
            player_who_went_out_ = current_player_;
            final_turns_remaining_ = game_.NumPlayers() - 1;

            if (final_turns_remaining_ == 0) {
                phase_ = Phase::Terminal;
            }
        }

        return;
    }

    if (final_turns_remaining_ > 0) {
        --final_turns_remaining_;
    }

    if (final_turns_remaining_ == 0) {
        phase_ = Phase::Terminal;
    }
}

void State::DealInitialCards() {
    players_.reserve(game_.NumPlayers());

    for (int player_id = 0; player_id < game_.NumPlayers(); ++player_id) {
        std::array<CardSlot, 12> hand;

        for (int card_index = 0; card_index < 12; ++card_index) {
            hand[card_index] = CardSlot{
                deck_.drawCard(DrawLocation::DrawPile),
                false
            };
        }

        players_.emplace_back(hand);
    }

    // Flip the top draw-pile card to start the discard pile.
    deck_.discardCard(deck_.drawCard(DrawLocation::DrawPile));
}

std::array<float, 15> State::ChanceOutcome() const {
    std::array<float, 15> dist{};

    for (int i = -2; i <= 12; ++i) {
        dist[i + 2] =
            static_cast<double>(current_seen_dist_.at(i)) /
            static_cast<double>(total_unseen_cards_);
    }

    return dist;
}

int State::SampleOutcome(std::mt19937& rng) const {
    std::uniform_int_distribution<int> dist(1, total_unseen_cards_);
    int target = dist(rng);

    int cumulative = 0;

    for (int value = -2; value <= 12; ++value) {
        cumulative += current_seen_dist_.at(value);

        if (target <= cumulative) {
            return value;
        }
    }
    throw std::runtime_error("SampleOutcome failed: invalid unseen card distribution");
}

std::map<int, int> State::CurrentSeenDist() const {
    return current_seen_dist_;
}

Game State::GameInstance() const {
    return game_;
}

int State::CurrentPlayer() const {
    return current_player_;
}

Phase State::CurrentPhase() const {
    return phase_;
}

std::string State::ToString() const {
    return "";
}

std::string State::ObservationString(int player) const {
    return "";
}
