//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>
#include "../include/skyjo/Deck.h"
#include "../include/skyjo/Card.h"

static std::vector<int> SortedValuesOf(const std::vector<Card>& cards) {
    std::vector<int> values;
    values.reserve(cards.size());

    for (const Card& card : cards) {
        values.push_back(card.value());
    }

    std::sort(values.begin(), values.end());
    return values;
}

TEST_CASE("Deck initializes with requested number of cards", "[deck]") {
    const Deck deck(150);
    REQUIRE(deck.drawPileSize() == 150);
}

TEST_CASE("Shuffle preserves draw pile size", "[deck]") {
    Deck deck(150);
    const int initial = deck.drawPileSize();

    deck.shuffle();
    const int post = deck.drawPileSize();

    REQUIRE(initial == post);
}

TEST_CASE("Shuffle preserves draw pile card values", "[deck]") {
    Deck deck(150);

    const std::vector<int> before = SortedValuesOf(deck.GetDrawPile());

    deck.shuffle();

    const std::vector<int> after = SortedValuesOf(deck.GetDrawPile());

    REQUIRE(after == before);
}

TEST_CASE("Drawing from draw pile decreases draw pile size by one", "[deck]") {
    constexpr int draw_pile_size = 150;
    Deck deck(draw_pile_size);

    (void) deck.drawCard(DrawLocation::DrawPile);

    REQUIRE(draw_pile_size - 1 == deck.drawPileSize());
}

TEST_CASE("Discarding a card increases discard pile size by one", "[deck]") {
    Deck deck(150);

    const int before = deck.discardPileSize();

    deck.discardCard(Card(7));

    REQUIRE(deck.discardPileSize() == before + 1);
}

TEST_CASE("Top discard card is the most recently discarded card", "[deck]") {
    Deck deck(150);

    const int before = deck.discardPileSize();

    const Card discard_card(5);

    deck.discardCard(Card(3));
    deck.discardCard(discard_card);

    REQUIRE(deck.discardTop() == discard_card);
}

TEST_CASE("Drawing from discard pile returns most recently discarded card", "[deck]") {
    Deck deck(150);

    deck.discardCard(Card(4));
    deck.discardCard(Card(11));

    const int before = deck.discardPileSize();

    const Card drawn = deck.drawCard(DrawLocation::DiscardPile);

    REQUIRE(drawn.value() == 11);
    REQUIRE(deck.discardPileSize() == before - 1);
}