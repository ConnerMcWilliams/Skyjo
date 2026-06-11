//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>
#include "../include/skyjo/Deck.h"
#include "../include/skyjo/Card.h"

TEST_CASE("Deck contains the right amount of cards after initial shuffle.", "[deck]") {
    Deck deck(150);
    const int initial = deck.drawPileSize();
    // There should be one less card than total, because there should be one in the discard pile.
    REQUIRE(initial == 150);
    deck.shuffle();
    // There should be no change in the total after the second shuffle.
    const int post = deck.drawPileSize();
    REQUIRE(initial == post);
}

TEST_CASE("Shuffle does not do the same shuffle twice.", "[deck]") {
    Deck deck(150);
    std::vector<Card> initial_draw_pile = deck.GetDrawPile();
    deck.shuffle();
    std::vector<Card> post_draw_pile = deck.GetDrawPile();
    REQUIRE(initial_draw_pile != post_draw_pile);
}

TEST_CASE("", "[deck]") {

};