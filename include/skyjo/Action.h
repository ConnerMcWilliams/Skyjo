//
// Created by Conner on 5/29/2026.
//

#ifndef SKYJO_ACTION_H
#define SKYJO_ACTION_H

using Action = int;
using PlayerId = int;

enum class ActionType {
    RevealInitialCard,
    DrawFromDeck,
    DrawFromDiscard,
    ReplaceCard,
    DiscardDrawnCardAndReveal,
    ChanceCardDraw,
    RealDraw
};

struct SkyjoAction {
    ActionType type;
    // position is value in the case of ChanceCardDraw
    int position = -1;
};

#endif //SKYJO_ACTION_H
