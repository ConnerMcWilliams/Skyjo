//
// Created by Conner on 5/28/2026.
//

#ifndef SKYJO_STATE_H
#define SKYJO_STATE_H
#include <optional>
#include <vector>

#include "Game.h"
#include "Player.h"
#include "Card.h"
#include "Action.h"
#include "Deck.h"

class Card;

enum class Phase {
    InitialReveal,
    ChooseDrawSource,
    ChooseReplaceOrDiscard,
    Terminal,
    ChanceDrawFromDeck
};

class State {
public:
    explicit State(const Game& game);

    [[nodiscard]] int CurrentPlayer() const;
    [[nodiscard]] std::array<CardSlot, 12> GetCurrentPlayerHand() const;
    [[nodiscard]] std::vector<Player> Players() const;

    [[nodiscard]] Phase CurrentPhase() const;
    [[nodiscard]] Card DrawnCard() const;

    [[nodiscard]] std::vector<Action> LegalActions() const;
    [[nodiscard]] std::map<int, int> CurrentSeenDist() const;
    [[nodiscard]] Card TopDiscardCard() const;
    void ApplyAction(Action action);

    // the float is the probability of the index
    [[nodiscard]] std::array<float, 15> ChanceOutcome() const;
    [[nodiscard]] int SampleOutcome(std::mt19937& rng) const;
    [[nodiscard]] bool IsChancePhase() const;

    [[nodiscard]] bool IsTerminal() const;
    [[nodiscard]] std::vector<int> Scores() const;

    [[nodiscard]] Game GameInstance() const;
    [[nodiscard]] Deck DeckInstance() const;

    [[nodiscard]] std::string ToString() const;
    [[nodiscard]] std::string ObservationString(int player) const;

private:
    const Game& game_;
    std::map<int, int> current_seen_dist_;
    int total_unseen_cards_;
    Deck deck_;
    std::vector<Player> players_;

    int current_player_ = 0;
    Phase phase_;
    std::optional<Card> drawn_card_;

    std::optional<PlayerId> player_who_went_out_;
    int final_turns_remaining_ = -1;
    int initial_turns_remaining_;

private:

    void DealInitialCards();
    void AdvanceTurn();
    void CheckForEndOfRound();

    [[nodiscard]] bool isValidPlayer(int player) const;

};


#endif //SKYJO_STATE_H
