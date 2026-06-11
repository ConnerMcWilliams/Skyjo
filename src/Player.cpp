//
// Created by Conner on 5/26/2026.
//

#include "../include/skyjo/Player.h"

#include <memory_resource>
#include <sstream>
#include <stdexcept>
#include <bit>

Player::Player(std::array<CardSlot, 12> cards)
    : cards_(std::move(cards)), num_cards_up_(0) {
    if (cards_.size() != 12) {
        throw std::runtime_error("Player must have exactly 12 cards");
    }

    column_mask_ = static_cast<std::uint16_t> (0U);

    valid_reveal_mask_ = static_cast<std::uint16_t>((1u << 12) - 1);;

    valid_replace_mask_ = static_cast<std::uint16_t>((1u << 12) - 1);

}

const std::optional<Card>& Player::CardAt(int position) const {
    ValidatePosition(position);
    return cards_[position].card;
}

void Player::ValidatePosition(const int position) const {
    if (position < 0 || position >= static_cast<int>(cards_.size())) {
        throw std::out_of_range("Player card position out of range");
    }
}

RevealResult Player::RevealCard(int column) {

    int row = FirstUnknownRow(column);
    const int position = 4*row + column;

    auto& slot = cards_[position];

    if (!slot.card.has_value()) {
        throw std::runtime_error("RevealCard tried to reveal an empty slot");
    }

    if (slot.face_up) {
        throw std::runtime_error("RevealCard tried to reveal an already face-up card");
    }

    slot.face_up = true;
    num_cards_up_++;
    column_mask_ |= 1u << (column * 4 + row);
    RemoveRevealPosition(position);

    RevealResult result{
        .revealed_card = *slot.card,
        .discarded_cards = {},
        .num_discarded = 0
    };

    if (ColumnFullyRevealed(column)) {
        if (CheckRemoveCards(column)) {
            auto& slot1 = cards_[column];
            auto& slot2 = cards_[column + 4];
            auto& slot3 = cards_[column + 8];

            result.discarded_cards[0] = *slot1.card;
            result.discarded_cards[1] = *slot2.card;
            result.discarded_cards[2] = *slot3.card;
            result.num_discarded = 3;

            slot1.card = std::nullopt;
            slot2.card = std::nullopt;
            slot3.card = std::nullopt;

            column_mask_ |= 1u << (column * 4 + 3);
        }
    }

    return result;
}

bool Player::ColumnFullyRevealed(int column) const {
    uint16_t column_bits = (column_mask_ >> (4 * column)) & 0b1111;
    return (column_bits & 0b0111) == 0b0111;
}

int Player::FirstUnknownRow(int column) const {
    const uint16_t revealed = (column_mask_ >> (4 * column)) & 0b0111;
    const uint16_t unknown = (~revealed) & 0b0111;

    if (unknown == 0) {
        throw std::runtime_error("No unknown card left in column");
    }

    return std::countr_zero(unknown);
}

std::array<CardSlot, 12> Player::GetHand() const {
    return cards_;
}

std::uint16_t Player::ValidRevealMask() const {
    return valid_reveal_mask_;
}

void Player::RemoveRevealPosition(const int position) {
    valid_replace_mask_ &= static_cast<uint16_t>(~(1u << position));
}

std::uint16_t Player::ValidReplaceMask() const {
    return valid_replace_mask_;
}

std::uint16_t Player::GetColumnMask() const {
    return column_mask_;
}

bool Player::CheckRemoveCards(const int column) const {
    auto&[card1, face_up1] = cards_[column];
    auto&[card2, face_up2] = cards_[column + 4];
    auto&[card3, face_up3] = cards_[column+8];
    if (card1.value() == card2.value() && card2.value() == card3.value()) {
        return true;
    }
    return false;
}

std::stack<Card> Player::ReplaceCard(const int position, const Card& new_card) {
    if (position < 0 || position >= static_cast<int>(cards_.size())) {
        throw std::out_of_range("ReplaceCard position out of range");
    }

    const int column = position % 4;
    const int row = position / 4;

    std::stack<Card> discard_cards;

    auto&[card, face_up] = cards_[position];

    if (!card.has_value()) {
        throw std::runtime_error("Player::ReplaceCard(): position invalid, trying to access an empty card slot.");
    }

    const Card old_card = card.value();
    discard_cards.push(old_card);
    card = new_card;
    if (!face_up) {
        face_up = true;
        num_cards_up_++;

        column_mask_ |= static_cast<uint16_t>(1u << (4 * column + row));
        RemoveRevealPosition(position);
    }


    if (ColumnFullyRevealed(column) && CheckRemoveCards(column)) {
        auto& slot1 = cards_[column];
        auto& slot2 = cards_[column + 4];
        auto& slot3 = cards_[column + 8];

        Card removed_1 = *slot1.card;
        Card removed_2 = *slot2.card;
        Card removed_3 = *slot3.card;

        slot1.card = std::nullopt;
        slot2.card = std::nullopt;
        slot3.card = std::nullopt;

        discard_cards.push(removed_1);
        discard_cards.push(removed_2);
        discard_cards.push(removed_3);

        column_mask_ |= 1u << (column * 4 + 3);
    }

    return discard_cards;
}

int Player::Score() const {
    int total = 0;

    if (cards_.size() != 12) {
        throw std::runtime_error("Player::Score(): player does not have 12 cards");
    }

    for (auto [card, face_up] : cards_) {
        if (!card.has_value()) {
            continue;
        }

        int value = card.value().value();

        if (value < -2 || value > 12) {
            throw std::runtime_error("Player::Score(): invalid card value");
        }

        total += value;
    }

    return total;
}

int Player::NumFaceUpCards() const {
    return num_cards_up_;
}

bool Player::AllCardsFaceUp() const {
    return num_cards_up_ == 12;
}

bool Player::IsFaceUp(const int position) const {
    auto [card, face_up] = cards_[position];
    return face_up;
}

std::string Player::ToString() const {
    std::ostringstream out;

    for (int i = 0; i < 12; ++i) {
        const auto&[card, face_up] = cards_[i];

        if (!card.has_value()) {
            out << "empty";
        }

        if (face_up) {
            out << card.value().value();
        } else {
            out << "X";
        }

        if ((i + 1) % 4 == 0) {
            out << '\n';
        } else {
            out << ' ';
        }
    }

    return out.str();
}
