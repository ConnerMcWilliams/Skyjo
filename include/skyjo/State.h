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

/**
 * @brief The different phases the state could be in.
 */
enum class Phase {
    InitialReveal,
    ChooseDrawSource,
    ChooseReplaceOrDiscard,
    Terminal,
    ChanceDrawFromDeck
};

/**
 * @class State
 *
 * @brief Tracks all the game and players' information with the current turn and phase.
 */
class State {
public:
    /// @brief Explicit State initializer with a specified game's rules
    /// @param game The specified game params and methods
    explicit State(const Game& game);

    /// @returns Returns the 0-indexed current player position.
    [[nodiscard]] int CurrentPlayer() const;
    /// @returns Returns the hand of the current player.
    // TODO: make non-revealed cards non-accessible to client
    [[nodiscard]] std::array<CardSlot, 12> GetCurrentPlayerHand() const;
    /// @returns An array of all the players in the state.
    [[nodiscard]] std::vector<Player> Players() const;

    /// @returns Returns the phase the state is currently in.
    [[nodiscard]] Phase CurrentPhase() const;
    /// @returns
    [[nodiscard]] Card DrawnCard() const;

    /// @returns Returns all the legal actions that the current player has.
    [[nodiscard]] std::vector<Action> LegalActions() const;
    /// @returns Returns a dictionary with all the cards that have been revealed to not be in the deck.
    [[nodiscard]] std::map<int, int> CurrentSeenDist() const;
    /// @returns The last card discarded.
    [[nodiscard]] Card TopDiscardCard() const;
    /// @remark Applies the specified action to the current state.
    /// @param action The action applied to the state.
    void ApplyAction(Action action);

    /// @returns Returns a float array where the index is the card value and the float at that index is the chance that
    /// card will be drawn from the deck.
    [[nodiscard]] std::array<float, 15> ChanceOutcome() const;
    /// @returns Returns a sample card value picked from the deck, picked at random from the remaining card
    /// distribution.
    [[nodiscard]] int SampleOutcome(std::mt19937& rng) const;
    /// @returns Returns whether the state is in a phase involving a random draw (drawing or revealing).
    [[nodiscard]] bool IsChancePhase() const;

    /// @returns Returns whether the game is over or not.
    [[nodiscard]] bool IsTerminal() const;

    /// @returns Returns a vector with all the player scores.
    /// @throw Throws UnaccessibleInformation error if game is not terminal
    // TODO: Make this not include revealed cards or throw if game is not terminal.
    [[nodiscard]] std::vector<int> Scores() const;

    /// @returns Returns the game instance that this state references.
    [[nodiscard]] Game GameInstance() const;
    /// @returns Returns the instance of Deck that this state uses.
    // TODO: Make sure this does not violate information hiding.
    [[nodiscard]] Deck DeckInstance() const;

    // TODO: implement the string methods.
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
