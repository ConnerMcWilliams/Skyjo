cmake_minimum_required(VERSION 3.24)
project(Skyjo)

set(CMAKE_CXX_STANDARD 20)

add_executable(Skyjo main.cpp
        src/Player.cpp
        include/skyjo/Player.h
        src/Game.cpp
        include/skyjo/Game.h
        src/Card.cpp
        include/skyjo/Card.h
        src/Deck.cpp
        include/skyjo/Deck.h
        src/State.cpp
        include/skyjo/State.h
        include/skyjo/Action.h
        include/agents/Agent.h
        src/agents/RandomAgent.cpp
        include/agents/RandomAgent.h
        src/agents/MinimaxAgent.cpp
        include/agents/MinimaxAgent.h
        src/experiments/Simulation.cpp
        include/experiments/Simulation.h
        src/experiments/Experiment.cpp
        include/experiments/Experiment.h
        src/agents/BasicLogic.cpp
        include/agents/BasicLogic.h
        src/experiments/SpeedExperiment.cpp
        include/experiments/SpeedExperiment.h
        src/agents/MonteCarloAgent.cpp
        include/agents/MonteCarloAgent.h
        src/experiments/Tournament.cpp
        include/experiments/Tournament.h
)

find_package(Catch2 CONFIG REQUIRED)
add_executable(tests
        tests/test_card.cpp
        src/Card.cpp
        tests/test_deck.cpp
        src/Deck.cpp
        tests/test_player.cpp
        src/Player.cpp
        tests/test_game.cpp
        src/Game.cpp
        tests/test_state.cpp
        src/State.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
