cmake_minimum_required(VERSION 3.24)
project(Skyjo)

set(CMAKE_CXX_STANDARD 20)

add_executable(Skyjo main.cpp
        src/player.cpp
        include/skyjo/player.h
        src/game.cpp
        include/skyjo/game.h
        src/card.cpp
        include/skyjo/card.h
        src/deck.cpp
        include/skyjo/deck.h
        src/state.cpp
        include/skyjo/state.h
        include/skyjo/Action.h
        include/skyjo/agents/Agent.h
        src/agents/RandomAgent.cpp
        include/skyjo/agents/RandomAgent.h
        src/agents/MinimaxAgent.cpp
        include/skyjo/agents/MinimaxAgent.h
        tests/Simulation.cpp
        include/skyjo/Simulation.h
        tests/Experiment.cpp
        include/skyjo/Experiment.h
        src/agents/BasicLogic.cpp
        include/skyjo/agents/BasicLogic.h
        tests/SpeedExperiment.cpp
        include/skyjo/SpeedExperiment.h
        src/agents/MonteCarloAgent.cpp
        include/skyjo/agents/MonteCarloAgent.h
        tests/Tournament.cpp
        include/skyjo/Tournament.h
)

find_package(Catch2 CONFIG REQUIRED)
add_executable(tests
        test-cases/test_card.cpp
        src/card.cpp
        test-cases/test_deck.cpp
        src/deck.cpp
        test-cases/test_player.cpp
        src/player.cpp
        test-cases/test_game.cpp
        src/game.cpp
        test-cases/test_state.cpp
        src/state.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
