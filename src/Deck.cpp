//
// Created by Conner on 5/28/2026.
//

#include "../include/skyjo/Deck.h"

#include <algorithm>
#include <ranges>
#include <stdexcept>

Deck::Deck(const unsigned short numCards, std::map<int, int> distribution,
           const unsigned int seed)
    : numCards_(numCards),
    distribution_(distribution),
    rng_(seed) {
    int total = 0;
    for (const auto &value: distribution | std::views::values) {
        if (value < 0) {
            throw std::invalid_argument("Card counts cannot be negative.");
        }

        total += value;
    }
    if (total != numCards) {
        throw std::invalid_argument(
            "The values in the distribution must add up to the number of cards."
            );
    }

    initializeDeck();
    shuffle();
}

void Deck::initializeDeck() {
    // Clears the stacks and makes sure they have the memory allocation to hold all cards.
    draw_pile_.clear();
    discard_pile_.clear();

    draw_pile_.reserve(numCards_);
    discard_pile_.reserve(numCards_);

    // adds each card to the deck.
    for (const auto& [value, count] : distribution_) {
        for (int i = 0; i < count; ++i) {
            draw_pile_.emplace_back(value);
        }
    }
}

void Deck::shuffle() {

    // If discard pile is already empty, shuffle the draw pile in place.
    if (discard_pile_.empty()) {
        std::ranges::shuffle(draw_pile_, rng_);
        return;
    }

    // Keep the top discard card visible.
    const Card top_discard = discard_pile_.back();
    discard_pile_.pop_back();

    // Move the rest of the discard pile into the draw pile.
    while (!discard_pile_.empty()) {
        draw_pile_.push_back(discard_pile_.back());
        discard_pile_.pop_back();
    }

    // Restore the top discard card.
    discard_pile_.push_back(top_discard);

    // Shuffle the recycled cards into the draw pile.
    std::ranges::shuffle(draw_pile_, rng_);
}

void Deck::discardCard(const Card& card) {
    discard_pile_.push_back(card);
}

Card Deck::drawCard(const DrawLocation location) {
    switch (location) {
        case DrawLocation::DrawPile : {
            if (draw_pile_.size() == 0) {
                shuffle();
            }
            if (draw_pile_.empty()) {
                throw std::out_of_range("drawCard: draw pile is empty");
            }
            const Card drawn_card = draw_pile_.back();
            draw_pile_.pop_back();
            return drawn_card;
        }

        case DrawLocation::DiscardPile : {
            if (discard_pile_.empty()) {
                throw std::out_of_range("drawCard: discard pile is empty");
            }
            const Card drawn_card = discard_pile_.back();
            discard_pile_.pop_back();
            return drawn_card;
        }
    }
    throw std::invalid_argument("Invalid draw location.");
}

const Card& Deck::discardTop() const {
    return discard_pile_.back();
}

bool Deck::drawPileEmpty() const {
    return draw_pile_.size() == 0;
}

bool Deck::discardPileEmpty() const {
    return discard_pile_.size() == 0;
}

int Deck::drawPileSize() const {
    return draw_pile_.size();
}

int Deck::discardPileSize() const {
    return discard_pile_.size();
}

std::map<int, int> Deck::distribution() const {
    return distribution_;
}

std::vector<Card> Deck::GetDrawPile() const {
    return draw_pile_;
}

std::vector<Card> Deck::GetDiscardPile() const {
    return discard_pile_;
}
