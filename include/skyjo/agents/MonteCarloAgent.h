//
// Created by conne on 6/8/2026.
//

#ifndef SKYJO_MONTECARLOAGENT_H
#define SKYJO_MONTECARLOAGENT_H
#include <memory>

#include "Agent.h"
#include "../Action.h"
#include "../State.h"

struct MCTSNode {
    State state;

    MCTSNode* parent = nullptr;
    Action action_from_parent = -1;

    double value = 0.0;
    int visits = 0;

    std::vector<std::unique_ptr<MCTSNode>> child_nodes;
    std::vector<Action> actions_not_taken;

    explicit MCTSNode (State state, MCTSNode* parent, Action action_from_parent) :
    state(state),
    parent(parent),
    action_from_parent(action_from_parent) {
        actions_not_taken = state.LegalActions();
    }

};

class MonteCarloAgent : public Agent {
public:
    explicit MonteCarloAgent(int num_simulations, double exploration_constant, unsigned int seed);

    [[nodiscard]] Action ChooseAction(const State& state) override;

private:

    int num_simulations_;
    double exploration_constant_;
    std::mt19937 rng_;

    MCTSNode* Select(MCTSNode* node, PlayerId root_player);
    double NodeEval(MCTSNode* node);

    MCTSNode* Expand(MCTSNode* node, PlayerId root_player);
    void ResolveNonRootAndChance(State& child_state, PlayerId root_player);

    double EvaluateTerminal(State state, PlayerId root_player);
    double Simulate(State state, PlayerId root_player);
    Action RolloutAction(const State& state, int threshold);
    Action RandomAction(const State& state);

    void BackPropagate(MCTSNode* node, double value);

    [[nodiscard]] MCTSNode* BestChildUCT(MCTSNode* node) const;
    [[nodiscard]] Action BestFinalAction(const MCTSNode& root) const;

};


#endif //SKYJO_MONTECARLOAGENT_H
