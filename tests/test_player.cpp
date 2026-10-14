//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>
#include "../include/skyjo/Player.h"
#include "../include/skyjo/Card.h"
#include <iostream>

namespace {

    std::array<CardSlot, 12> MakeSequentialHand() {
        std::array<CardSlot, 12> hand{};

        for (int i = 0; i < 12; ++i) {
            hand[i] = CardSlot(Card(i), false);
        }

        return hand;
    }

    Player MakeSequentialPlayer() {
        return Player(MakeSequentialHand());
    }

} // namespace

TEST_CASE("Test that reveal card reveals the right card.", "[player]") {
    Player player = MakeSequentialPlayer();

    (void) player.RevealCard(0);
    (void) player.RevealCard(6);

    REQUIRE(player.GetHand()[0].face_up == true);
    REQUIRE(player.GetHand()[6].face_up == true);
    REQUIRE(player.GetHand()[1].face_up == false);
}

TEST_CASE("Test that replace card replaces the right card.", "[player]") {
    std::array<CardSlot, 12> hand;
    for (int i = 0; i < 12; ++i) {
        hand[i] = CardSlot(Card(i), false);
    }
    Player player(hand);

    (void) player.ReplaceCard(2, Card(12));
    REQUIRE(player.CardAt(2) == Card(12));
}

TEST_CASE("Test that a column with the same values gets removed.", "[player]") {
    std::array<CardSlot, 12> hand;
    for (int i = 0; i < 12; ++i) {
        hand[i] = CardSlot(Card(0), false);
    }
    Player player(hand);

    std::optional<Card> card_1 = player.CardAt(0);
    std::optional<Card> card_2 = player.CardAt(4);
    std::optional<Card> card_3 = player.CardAt(8);

    REQUIRE(card_1.has_value());
    REQUIRE(card_2.has_value());
    REQUIRE(card_3.has_value());

    (void) player.RevealCard(0);
    (void) player.RevealCard(0);
    (void) player.ReplaceCard(8, Card(0));

    card_1 = player.CardAt(0);
    card_2 = player.CardAt(4);
    card_3 = player.CardAt(8);

    REQUIRE(!card_1.has_value());
    REQUIRE(!card_2.has_value());
    REQUIRE(!card_3.has_value());

    REQUIRE(player.NumFaceUpCards() == 3);
}