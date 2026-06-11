//
// Created by Conner on 6/5/2026.
//

#include "../include/skyjo/Game.h"
#include "../include/skyjo/State.h"
#include<catch2/catch_test_macros.hpp>

TEST_CASE("Make sure the initial reveal works correctly.", "[state]") {

    const Game game(2);
    State state(game);

    std::vector<Action> legal_actions = state.LegalActions();
    std::vector tracking_actions {
        game.SerializeAction(SkyjoAction(ActionType::RevealInitialCard, 0)),
        game.SerializeAction(SkyjoAction(ActionType::RevealInitialCard, 1))
    };

    REQUIRE(legal_actions == tracking_actions);

    // Reveal player ones card at 1
    state.ApplyAction(legal_actions[0]);
    // Reveal player twos card at 2
    state.ApplyAction(legal_actions[1]);

    // Player 1's actions are still good
    REQUIRE(state.LegalActions() == tracking_actions);

    std::array<CardSlot, 12> hand = state.GetCurrentPlayerHand();
    for (int i = 1; i<12; i++) {
        REQUIRE(hand[i].face_up == false);
    }
    REQUIRE(hand[0].face_up == true);

    // Reveal second card in the first row to make it Player 2's turn.
    state.ApplyAction(tracking_actions[0]);

    REQUIRE(state.CurrentPhase() == Phase::InitialReveal);

    std::array<CardSlot, 12> second_hand = state.GetCurrentPlayerHand();
    for (int i = 0; i<12; i++) {
        if (i == 1) {
            continue;
        }
        REQUIRE(second_hand[i].face_up == false);
    }
    REQUIRE(second_hand[1].face_up == true);

    legal_actions = state.LegalActions();
    state.ApplyAction(legal_actions[0]);

    // After all the reveal cards make sure it is now not in the initial stage anymore
    REQUIRE(state.CurrentPhase() == Phase::ChooseDrawSource);
}

TEST_CASE(
    "Make sure current distribution is updated correctly after a card is drawn or revealed.",
    "[state]"
    ) {
    // Update this to include initial phase.

    const Game game(2);
    State state(game);
    std::map<int, int> init_distribution = state.CurrentSeenDist();
    REQUIRE(init_distribution == game.distribution());
    // Action 0 is draw from deck
    state.ApplyAction(0);
    const Card drawn_card = state.DrawnCard();
    const int drawn_card_value = drawn_card.value();
    init_distribution[drawn_card_value]--;
    REQUIRE(init_distribution == state.CurrentSeenDist());
}

TEST_CASE("Make sure removing the triple works.", "[state]") {

}

TEST_CASE("Make sure the game ends at the right time.", "[state]") {
    const Game game(2);
    State state(game);

    std::vector<Action> legal_actions = state.LegalActions();
    state.ApplyAction(legal_actions[0]);
    legal_actions = state.LegalActions();
    state.ApplyAction(legal_actions[0]);

    REQUIRE(state.CurrentPlayer() == 0);

    legal_actions = state.LegalActions();
    state.ApplyAction(legal_actions[0]);
    legal_actions = state.LegalActions();
    state.ApplyAction(legal_actions[0]);

    // Out of initial phase, two cards revealed. Reveal the next ten cards.
    for (int i = 1; i <= 10; ++i) {

    }

}
