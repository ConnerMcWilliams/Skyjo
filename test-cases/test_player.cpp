//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>
#include "../include/skyjo/Player.h"
#include "../include/skyjo/Card.h"
#include <iostream>

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
    (void) player.RevealCard(4);
    (void) player.ReplaceCard(8, Card(0));

    card_1 = player.CardAt(0);
    card_2 = player.CardAt(4);
    card_3 = player.CardAt(8);

    std::array<CardSlot, 12> curr_hand = player.GetHand();
    for (int i = 0; i < 12; ++i) {
        std::cout << curr_hand[i].card.value().toString();
    }

    REQUIRE(!card_1.has_value());
    REQUIRE(!card_2.has_value());
    REQUIRE(!card_3.has_value());

    REQUIRE(player.NumFaceUpCards() == 3);
}