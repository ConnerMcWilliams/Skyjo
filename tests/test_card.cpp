//
// Created by Conner on 6/4/2026.
//

#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

#include "../include/skyjo/Card.h"

TEST_CASE("Instantiation and getters work", "[card]") {
    const Card card(2);
    const int value = card.value();
    REQUIRE(value == 2);
}

TEST_CASE("Check comparison operators", "[card]") {
    const Card card_1(1);
    const Card card_2(1);
    const Card card_3(2);

    REQUIRE(card_1 == card_2);
    REQUIRE(card_1 != card_3);

    // Add greater than less than
}

TEST_CASE("Card rejects values below -2", "[card]") {
    REQUIRE_THROWS_AS(Card(-3), std::invalid_argument);
}

TEST_CASE("Card rejects values above 12", "[card]") {
    REQUIRE_THROWS_AS(Card(13), std::invalid_argument);
}