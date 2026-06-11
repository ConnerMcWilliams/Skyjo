//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <set>
#include <vector>

#include "../include/skyjo/Game.h"
#include "../include/skyjo/State.h"
#include "../include/skyjo/Action.h"

TEST_CASE("Game stores number of players", "[game]") {
    Game game(4);

    REQUIRE(game.NumPlayers() == 4);
}

TEST_CASE("Game rejects invalid number of players", "[game]") {
    REQUIRE_THROWS_AS(Game(0), std::invalid_argument);
    REQUIRE_THROWS_AS(Game(1), std::invalid_argument);
}

TEST_CASE("Game serializes and deserializes draw source actions", "[game]") {
    Game game(2);

    {
        SkyjoAction original{ActionType::DrawFromDeck, -1};
        Action encoded = game.SerializeAction(original);
        SkyjoAction decoded = game.DeserializeAction(encoded);

        REQUIRE(decoded.type == ActionType::DrawFromDeck);
        REQUIRE(decoded.position == -1);
    }

    {
        SkyjoAction original{ActionType::DrawFromDiscard, -1};
        Action encoded = game.SerializeAction(original);
        SkyjoAction decoded = game.DeserializeAction(encoded);

        REQUIRE(decoded.type == ActionType::DrawFromDiscard);
        REQUIRE(decoded.position == -1);
    }
}

TEST_CASE("Game serializes and deserializes all board-position actions", "[game]") {
    Game game(2);

    for (int pos = 0; pos < 12; ++pos) {
        {
            SkyjoAction original{ActionType::ReplaceCard, pos};
            Action encoded = game.SerializeAction(original);
            SkyjoAction decoded = game.DeserializeAction(encoded);

            REQUIRE(decoded.type == ActionType::ReplaceCard);
            REQUIRE(decoded.position == pos);
        }
    }
    for (int pos = 0; pos < 4; ++pos) {
        SkyjoAction original{ActionType::DiscardDrawnCardAndReveal, pos};
        Action encoded = game.SerializeAction(original);
        SkyjoAction decoded = game.DeserializeAction(encoded);

        REQUIRE(decoded.type == ActionType::DiscardDrawnCardAndReveal);
        REQUIRE(decoded.position == pos);
    }

    for (int value = -2; value < 12; ++value) {
        SkyjoAction original{ActionType::ChanceCardDraw, value};
        Action encoded = game.SerializeAction(original);
        SkyjoAction decoded = game.DeserializeAction(encoded);

        REQUIRE(decoded.type == ActionType::ChanceCardDraw);
        REQUIRE(decoded.position == value);
    }
}

TEST_CASE("Game action serialization produces unique action ids", "[game]") {
    Game game(2);

    std::set<Action> actions;

    actions.insert(game.SerializeAction({ActionType::DrawFromDeck, -1}));
    actions.insert(game.SerializeAction({ActionType::DrawFromDiscard, -1}));

    for (int pos = 0; pos < 12; ++pos) {
        actions.insert(game.SerializeAction({ActionType::ReplaceCard, pos}));
    }

    for (int pos = 0; pos < 4; ++pos) {
        actions.insert(game.SerializeAction({ActionType::DiscardDrawnCardAndReveal, pos}));
    }

    actions.insert(game.SerializeAction({ActionType::RevealInitialCard, 0}));

    actions.insert(game.SerializeAction({ActionType::RevealInitialCard, 1}));

    for (int value = -2; value < 12; ++value) {
        actions.insert(game.SerializeAction({ActionType::ChanceCardDraw, value}));
    }

    actions.insert(game.SerializeAction({ActionType::RealDraw, -1}));

    REQUIRE(actions.size() == 35);
}

TEST_CASE("Game rejects invalid board positions during serialization", "[game]") {
    Game game(2);

    REQUIRE_THROWS_AS(
        game.SerializeAction({ActionType::ReplaceCard, -1}),
        std::invalid_argument
    );

    REQUIRE_THROWS_AS(
        game.SerializeAction({ActionType::ReplaceCard, 12}),
        std::invalid_argument
    );

    REQUIRE_THROWS_AS(
        game.SerializeAction({ActionType::DiscardDrawnCardAndReveal, -1}),
        std::invalid_argument
    );

    REQUIRE_THROWS_AS(
        game.SerializeAction({ActionType::DiscardDrawnCardAndReveal, 12}),
        std::invalid_argument
    );

    REQUIRE_THROWS_AS(
        game.SerializeAction({ActionType::RevealInitialCard, -1}),
        std::invalid_argument
    );

    REQUIRE_THROWS_AS(
        game.SerializeAction({ActionType::RevealInitialCard, 12}),
        std::invalid_argument
    );
}

TEST_CASE("Game rejects invalid serialized action ids", "[game]") {
    Game game(2);

    REQUIRE_THROWS_AS(game.DeserializeAction(-1), std::invalid_argument);
    REQUIRE_THROWS_AS(game.DeserializeAction(38), std::invalid_argument);
    REQUIRE_THROWS_AS(game.DeserializeAction(1000), std::invalid_argument);
}