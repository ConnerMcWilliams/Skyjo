//
// Created by Conner on 5/26/2026.
//

#ifndef SKYJO_PLAYER_H
#define SKYJO_PLAYER_H

#include <array>
#include <cstdint>

#include "./Card.h"

#include <optional>
#include <set>
#include <stack>
#include <vector>

struct CardSlot {
    std::optional<Card> card;
    bool face_up = false;
};

struct RevealResult {
    Card revealed_card;
    std::array<std::optional<Card>, 3> discarded_cards;
    int num_discarded = 0;

    [[nodiscard]] bool ClearedColumn() const {
        return num_discarded == 3;
    }
};

class Player {
public:
    Player() = default;
    explicit Player(std::array<CardSlot, 12> cards);

    [[nodiscard]] const std::optional<Card>& CardAt(int position) const;
    [[nodiscard]] std::array<CardSlot, 12> GetHand() const;

    [[nodiscard]] RevealResult RevealCard(int position);
    [[nodiscard]] std::stack<Card> ReplaceCard(int position, const Card& new_card);

    [[nodiscard]] bool IsFaceUp(int position) const;
    [[nodiscard]] int Score() const;

    [[nodiscard]] std::uint16_t ValidRevealMask() const;
    [[nodiscard]] std::uint16_t ValidReplaceMask() const;
    [[nodiscard]] std::uint16_t GetColumnMask() const;
    [[nodiscard]] int NumFaceUpCards() const;
    [[nodiscard]] bool AllCardsFaceUp() const;

    [[nodiscard]] std::string ToString() const;

private:
    std::array<CardSlot, 12> cards_;
    int num_cards_up_;
    std::uint16_t valid_reveal_mask_;
    std::uint16_t valid_replace_mask_;
    std::uint16_t column_mask_;


private:
    void ValidatePosition(int position) const;
    int FirstUnknownRow(int position) const;
    void RemoveRevealPosition(int position);
    bool ColumnFullyRevealed(int column) const;
    bool CheckRemoveCards(int column) const;
};

#endif //SKYJO_PLAYER_H
