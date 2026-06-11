//
// Created by Conner on 5/28/2026.
//

#ifndef SKYJO_CARD_H
#define SKYJO_CARD_H
#include <string>


class Card final {
public:
    explicit Card(int value);
    [[nodiscard]] int value() const;
    [[nodiscard]] std::string toString() const;

    bool operator==(const Card &) const;
    bool operator!=(const Card &) const;
private:
    int value_;
};


#endif //SKYJO_CARD_H
