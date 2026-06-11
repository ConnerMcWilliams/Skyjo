//
// Created by Conner on 5/28/2026.
//

#ifndef SKYJO_DECK_H
#define SKYJO_DECK_H
#include <map>
#include <random>
#include <vector>

#include "Card.h"

enum class DrawLocation {
    DrawPile,
    DiscardPile
};

const std::map<int, int> DEFAULT_DIST = {
    {-2, 5},
    {-1, 10},
    {0, 15},
    {1, 10},
    {2, 10},
    {3, 10},
    {4, 10},
    {5, 10},
    {6, 10},
    {7, 10},
    {8, 10},
    {9, 10},
    {10, 10},
    {11, 10},
    {12, 10}
};

class Deck {
public:
    explicit Deck(unsigned short numCards, std::map<int, int> distribution = DEFAULT_DIST,
        unsigned int seed = std::random_device{}());

    [[nodiscard]] bool drawPileEmpty() const;
    [[nodiscard]] bool discardPileEmpty() const;

    [[nodiscard]] int drawPileSize() const;
    [[nodiscard]] int discardPileSize() const;

    [[nodiscard]] const Card& discardTop() const;

    Card drawCard(DrawLocation location);
    void discardCard(const Card& card);

    void shuffle();

    [[nodiscard]] std::map<int, int> distribution() const;

    // For testing
    [[nodiscard]] std::vector<Card> GetDrawPile() const;
    [[nodiscard]] std::vector<Card> GetDiscardPile() const;

private:
    unsigned short numCards_;
    std::map<int, int> distribution_;

    std::vector<Card> draw_pile_;
    std::vector<Card> discard_pile_;

    // mt19937 is a pseudo-random number generator
    std::mt19937 rng_;

    void initializeDeck();
};


#endif //SKYJO_DECK_H
