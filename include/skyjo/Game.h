//
// Created by Conner on 5/26/2026.
//

#ifndef SKYJO_GAME_H
#define SKYJO_GAME_H

#include "./Deck.h"

#include <map>
#include <vector>
#include "Action.h"

class Game {
public:
    explicit Game(int numPlayers, std::map<int, int> distribution=DEFAULT_DIST);

    [[nodiscard]] int NumPlayers() const;
    [[nodiscard]] int NumDistinctActions();
    [[nodiscard]] const std::map<int, int> &distribution() const;

    [[nodiscard]] Action SerializeAction(const SkyjoAction& action) const;
    [[nodiscard]] SkyjoAction DeserializeAction(Action action) const;

private:
    int num_players_;

    std::map<int, int> distribution_;
};


#endif //SKYJO_GAME_H
