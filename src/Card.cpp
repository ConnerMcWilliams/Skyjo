//
// Created by conne on 5/28/2026.
//

#include "../include/skyjo/Card.h"

#include <functional>
#include <stdexcept>
#include <bits/error_constants.h>

Card::Card(const int value) : value_(value) {
    if (value_ < -2 || value > 12) {
        throw std::invalid_argument(
            "Skyjo card value must be between -2 and 12."
            );
    }
}

int Card::value() const {
    return value_;
}

std::string Card::toString() const {
    return std::to_string(value_);
}

bool Card::operator==(const Card& other) const {
    return value_ == other.value_;
}

bool Card::operator!=(const Card& other) const {
    return !(*this == other);
}