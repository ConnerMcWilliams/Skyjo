//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>

#include "../include/skyjo/Action.h"
#include "../include/skyjo/Game.h"
#include "../include/skyjo/State.h"

TEST_CASE(
    "Initial state starts in the initial-reveal phase",
    "[state][initialization]"
) {
    Game game(2);
    State state(game);

    REQUIRE(state.CurrentPhase() == Phase::InitialReveal);
}

TEST_CASE(
    "Initial state starts with player zero as the current player",
    "[state][initialization]"
) {
    Game game(2);
    State state(game);

    REQUIRE(state.CurrentPlayer() == 0);
}

TEST_CASE(
    "Initial state is not terminal",
    "[state][initialization]"
) {
    Game game(2);
    State state(game);

    REQUIRE_FALSE(state.IsTerminal());
}

TEST_CASE(
    "Initial state deals twelve cards to every player",
    "[state][initialization]"
) {
    Game game(2);
    State state(game);

    REQUIRE(state.Players().size() == 2);

    for (const Player& player : state.Players()) {
        const auto& hand = player.GetHand();

        REQUIRE(hand.size() == 12);

        for (const CardSlot& slot : hand) {
            REQUIRE(slot.card.has_value());
        }
    }
}

TEST_CASE(
    "Initial state begins with all player cards face down",
    "[state][initialization]"
) {
    Game game(2);
    State state(game);

    for (const Player& player : state.Players()) {
        REQUIRE(player.NumFaceUpCards() == 0);

        for (const CardSlot& slot : player.GetHand()) {
            REQUIRE_FALSE(slot.face_up);
        }
    }
}

TEST_CASE(
    "Initial state removes the dealt cards from the draw pile",
    "[state][initialization][deck]"
) {
    constexpr int deck_size = 150;
    constexpr int num_players = 2;
    constexpr int cards_per_player = 12;

    Game game(num_players);
    State state(game);

    const int expected_draw_pile_size =
        deck_size - num_players * cards_per_player - 1;

    REQUIRE(
        state.DeckInstance().drawPileSize()
        == expected_draw_pile_size
    );
}

TEST_CASE(
    "Initial reveal phase provides one action for every face-down position",
    "[state][legal-actions][initial-reveal]"
) {
    // Verify that the current player initially has twelve legal reveal actions.
}

TEST_CASE(
    "Initial reveal legal actions contain only reveal actions",
    "[state][legal-actions][initial-reveal]"
) {
    // Deserialize every legal action and verify that each has the correct action type.
}

TEST_CASE(
    "Initial reveal legal actions contain each board position exactly once",
    "[state][legal-actions][initial-reveal]"
) {
    // Verify that positions zero through eleven appear once with no duplicates.
}

TEST_CASE(
    "Applying an initial reveal action reveals the selected card",
    "[state][apply-action][initial-reveal]"
) {
    // Apply a reveal action and verify that the selected card becomes face up.
}

TEST_CASE(
    "Applying an initial reveal action does not reveal other cards",
    "[state][apply-action][initial-reveal]"
) {
    // Verify that only the selected board position changes visibility.
}

TEST_CASE(
    "Revealed initial position is removed from legal actions",
    "[state][legal-actions][initial-reveal]"
) {
    // Reveal one card and verify that the same position cannot be revealed again.
}

TEST_CASE(
    "Player remains current until completing required initial reveals",
    "[state][turns][initial-reveal]"
) {
    // Apply fewer than the required number of initial reveals and verify no turn change.
}

TEST_CASE(
    "Initial reveal advances to the next player after required reveals",
    "[state][turns][initial-reveal]"
) {
    // Complete one player's required initial reveals and verify CurrentPlayer() advances.
}

TEST_CASE(
    "Initial reveal wraps from the final player to player zero",
    "[state][turns][initial-reveal]"
) {
    // Complete initial reveals for the final player and verify turn order wraps correctly.
}

TEST_CASE(
    "Completing all initial reveals enters choose-draw-source phase",
    "[state][phase][initial-reveal]"
) {
    // Complete every player's initial reveals and verify the normal turn phase begins.
}

TEST_CASE(
    "Normal turn begins with the correct starting player",
    "[state][turns]"
) {
    // Verify which player begins after all initial reveals have completed.
}

TEST_CASE(
    "Choose-draw-source phase provides deck and discard actions",
    "[state][legal-actions][draw-source]"
) {
    // Verify that drawing from the deck and drawing from discard are both legal.
}

TEST_CASE(
    "Choose-draw-source phase contains no board-position actions",
    "[state][legal-actions][draw-source]"
) {
    // Verify that replace and reveal actions are not legal before drawing.
}

TEST_CASE(
    "Drawing from the deck stores the drawn card",
    "[state][apply-action][draw]"
) {
    // Apply DrawFromDeck and verify that State records a drawn card.
}

TEST_CASE(
    "Drawing from the deck decreases draw-pile size by one",
    "[state][apply-action][draw][deck]"
) {
    // Compare the draw-pile size before and after DrawFromDeck.
}

TEST_CASE(
    "Drawing from the deck enters replace-or-discard phase",
    "[state][phase][draw]"
) {
    // Verify that drawing changes the phase to ChooseReplaceOrDiscard.
}

TEST_CASE(
    "Drawing from discard stores the previous top discard card",
    "[state][apply-action][draw][discard]"
) {
    // Record the top discard, draw it, and verify it becomes the state's drawn card.
}

TEST_CASE(
    "Drawing from discard decreases discard-pile size by one",
    "[state][apply-action][draw][discard]"
) {
    // Compare discard-pile size before and after DrawFromDiscard.
}

TEST_CASE(
    "Drawing from discard enters replace phase",
    "[state][phase][draw][discard]"
) {
    // Verify the phase reached after taking the visible discard card.
}

TEST_CASE(
    "Card drawn from discard cannot immediately be discarded again",
    "[state][legal-actions][discard]"
) {
    // Verify Skyjo's rule that a discard-pile draw must replace a player card.
}

TEST_CASE(
    "Card drawn from deck may replace any occupied player position",
    "[state][legal-actions][replace]"
) {
    // Verify that all valid occupied positions produce ReplaceCard actions.
}

TEST_CASE(
    "Card drawn from deck may be discarded while revealing a face-down card",
    "[state][legal-actions][discard-reveal]"
) {
    // Verify that each valid face-down position produces a discard-and-reveal action.
}

TEST_CASE(
    "Discard-and-reveal actions exclude already face-up positions",
    "[state][legal-actions][discard-reveal]"
) {
    // Make a card face up and verify it is not offered as a reveal target.
}

TEST_CASE(
    "Replacement actions exclude removed card positions",
    "[state][legal-actions][replace]"
) {
    // Remove a column and verify its empty positions cannot be selected for replacement.
}

TEST_CASE(
    "Discard-and-reveal actions exclude removed card positions",
    "[state][legal-actions][discard-reveal]"
) {
    // Remove a column and verify empty positions cannot be selected for revealing.
}

TEST_CASE(
    "Replacing a card installs the drawn card at the selected position",
    "[state][apply-action][replace]"
) {
    // Draw a card, replace a selected slot, and verify the drawn card occupies that slot.
}

TEST_CASE(
    "Replacing a card makes the selected position face up",
    "[state][apply-action][replace]"
) {
    // Replace a face-down card and verify that its position becomes face up.
}

TEST_CASE(
    "Replacing a card moves the old card to the discard pile",
    "[state][apply-action][replace][discard]"
) {
    // Record the old card and verify it becomes the top discard after replacement.
}

TEST_CASE(
    "Replacing a card clears the stored drawn card",
    "[state][apply-action][replace]"
) {
    // Verify that State no longer contains a drawn card after replacement completes.
}

TEST_CASE(
    "Replacing a card advances to the next player",
    "[state][turns][replace]"
) {
    // Complete a replacement and verify CurrentPlayer() advances exactly once.
}

TEST_CASE(
    "Replacing a card returns the state to choose-draw-source phase",
    "[state][phase][replace]"
) {
    // Verify that the next player's turn begins in ChooseDrawSource.
}

TEST_CASE(
    "Discarding a drawn card places it on top of the discard pile",
    "[state][apply-action][discard-reveal]"
) {
    // Draw from the deck, discard that card, and verify it becomes the top discard.
}

TEST_CASE(
    "Discarding a drawn card reveals the selected player card",
    "[state][apply-action][discard-reveal]"
) {
    // Verify that the requested face-down card becomes visible.
}

TEST_CASE(
    "Discarding and revealing clears the stored drawn card",
    "[state][apply-action][discard-reveal]"
) {
    // Verify that the temporary drawn card is cleared after the action.
}

TEST_CASE(
    "Discarding and revealing advances to the next player",
    "[state][turns][discard-reveal]"
) {
    // Complete the action and verify CurrentPlayer() advances.
}

TEST_CASE(
    "Discarding and revealing returns to choose-draw-source phase",
    "[state][phase][discard-reveal]"
) {
    // Verify that the next turn starts in ChooseDrawSource.
}

TEST_CASE(
    "Completing a matching column removes all three cards",
    "[state][apply-action][column]"
) {
    // Set up two matching face-up cards, replace the third, and verify the column clears.
}

TEST_CASE(
    "Removed column cards are added to the discard pile",
    "[state][apply-action][column][discard]"
) {
    // Verify that every card removed from a completed column is discarded correctly.
}

TEST_CASE(
    "Completing a column does not remove cards from another column",
    "[state][apply-action][column]"
) {
    // Clear one column and verify every unrelated position remains unchanged.
}

TEST_CASE(
    "State rejects an action that is not currently legal",
    "[state][validation]"
) {
    // Attempt to apply an action from the wrong phase and verify that it is rejected.
}

TEST_CASE(
    "State rejects a replacement before a card has been drawn",
    "[state][validation][replace]"
) {
    // Attempt ReplaceCard during ChooseDrawSource and verify failure.
}

TEST_CASE(
    "State rejects drawing twice during the same turn",
    "[state][validation][draw]"
) {
    // Draw once, then attempt another draw before resolving the first card.
}

TEST_CASE(
    "State rejects revealing an already face-up card",
    "[state][validation][reveal]"
) {
    // Attempt a discard-and-reveal action targeting a face-up position.
}

TEST_CASE(
    "State rejects actions targeting positions outside the board",
    "[state][validation]"
) {
    // Attempt actions using positions below zero or above eleven.
}

TEST_CASE(
    "State rejects applying player actions after the game is terminal",
    "[state][validation][terminal]"
) {
    // Reach a terminal state and verify that further actions cannot be applied.
}

TEST_CASE(
    "Current player wraps correctly after the final player",
    "[state][turns]"
) {
    // Complete the final player's turn and verify the next player is player zero.
}

TEST_CASE(
    "Only the current player's board changes during an action",
    "[state][players]"
) {
    // Apply an action and verify that all non-current player boards remain unchanged.
}

TEST_CASE(
    "A player completing their board starts the final-turn sequence",
    "[state][end-game]"
) {
    // Reveal or replace the final unresolved card and verify end-game tracking begins.
}

TEST_CASE(
    "The player who finishes first is recorded",
    "[state][end-game]"
) {
    // Complete one player's board and verify State stores that player's ID.
}

TEST_CASE(
    "Players receive their final turn after another player finishes",
    "[state][end-game][turns]"
) {
    // Verify that remaining players each receive the intended final turn.
}

TEST_CASE(
    "The player who finishes first does not receive an extra final turn",
    "[state][end-game][turns]"
) {
    // Verify that turn order ends before returning to the player who triggered the ending.
}

TEST_CASE(
    "State becomes terminal after all required final turns",
    "[state][end-game][terminal]"
) {
    // Complete the final-turn sequence and verify IsTerminal() becomes true.
}

TEST_CASE(
    "State does not become terminal before all final turns are complete",
    "[state][end-game][terminal]"
) {
    // Verify that IsTerminal() remains false during the intermediate final turns.
}

TEST_CASE(
    "Terminal state has no legal player actions",
    "[state][legal-actions][terminal]"
) {
    // Reach a terminal state and verify LegalActions() is empty.
}

TEST_CASE(
    "Returns provides one result for every player",
    "[state][returns]"
) {
    // Reach a terminal state and verify Returns().size() equals NumPlayers().
}

TEST_CASE(
    "Returns reflects each player's final score",
    "[state][returns]"
) {
    // Construct known final boards and verify each return matches the scoring convention.
}

TEST_CASE(
    "Returns ranks lower Skyjo scores as better outcomes",
    "[state][returns]"
) {
    // Verify that a player with a lower final score receives a better return.
}

TEST_CASE(
    "Returns handles tied player scores",
    "[state][returns]"
) {
    // Give multiple players equal final scores and verify tie behavior.
}

TEST_CASE(
    "Returns rejects being requested before the state is terminal",
    "[state][returns][validation]"
) {
    // Decide whether non-terminal Returns() should throw or return a defined placeholder.
}

TEST_CASE(
    "State copy preserves all observable game information",
    "[state][copy]"
) {
    // Copy a state and compare phase, player, boards, piles, and drawn card.
}

TEST_CASE(
    "Mutating a copied state does not mutate the original state",
    "[state][copy]"
) {
    // Apply an action to a copied state and verify the source state remains unchanged.
}

TEST_CASE(
    "Applying the same action to identical states produces identical results",
    "[state][determinism]"
) {
    // Copy a state, apply the same deterministic action to both, and compare results.
}

TEST_CASE(
    "LegalActions never returns duplicate action IDs",
    "[state][legal-actions]"
) {
    // Check the legal-action vector for duplicate serialized actions in every phase.
}

TEST_CASE(
    "Every action returned by LegalActions can be applied successfully",
    "[state][legal-actions][validation]"
) {
    // Copy the state for each legal action and verify applying that action does not throw.
}

TEST_CASE(
    "Player actions preserve the total number of cards in the game",
    "[state][invariant][cards]"
) {
    // Count cards across players, draw pile, discard pile, and drawn-card storage before
    // and after actions, verifying that no cards are created or lost.
}

TEST_CASE(
    "At most one drawn card exists between drawing and resolving a turn",
    "[state][invariant][drawn-card]"
) {
    // Verify the optional drawn-card state is populated only in the correct phase.
}

TEST_CASE(
    "State phase and drawn-card presence remain consistent",
    "[state][invariant][phase]"
) {
    // Verify that ChooseReplaceOrDiscard has a drawn card and ChooseDrawSource does not.
}

TEST_CASE(
    "Current player always refers to a valid player ID",
    "[state][invariant][players]"
) {
    // Advance through many turns and verify CurrentPlayer() remains within bounds.
}

TEST_CASE(
    "A complete game can be played using only legal actions",
    "[state][integration]"
) {
    // Repeatedly select a legal action until terminal and verify no invalid transition occurs.
}

TEST_CASE(
    "A complete game terminates within a reasonable action limit",
    "[state][integration]"
) {
    // Play legal actions with a fixed policy and guard against an infinite game loop.
}

TEST_CASE(
    "State observation hides face-down cards from other players",
    "[state][observation]"
) {
    // Verify ObservationString() does not expose hidden card values.
}

TEST_CASE(
    "State observation shows face-up cards",
    "[state][observation]"
) {
    // Reveal known cards and verify they appear in the player's observation.
}

TEST_CASE(
    "State string includes current player and phase information",
    "[state][string]"
) {
    // Verify ToString() includes enough state information to support debugging.
}