# Overview
This project is a high performance C++ Skyjo Card game simulator.
The card game Skyjo revolves around players getting a hand of twelve
faced down cards that are slowly revealed and exchanged throughout the match.
The goal is for the total score of this hand to be minimized.
The main purpose of this simulator is to test the various agents that have been created
and to use game theory to find an optimal strategy. 
Currently, there are four main agents for testing: random, basic logic, expectiminimax, and monte-carlo decision tree.

## Features
1. A multithreaded simulation that leverages the speed of c++ to test advanced agents on.
2. Independent structure in which the states and their corresponding player states are independent of other states.
3. Tournament experiment that uses a Bradley-Terry model to give agents elo
4. Reproducible experiment runner
5. Random and rule-based agents
6. Win-rate and average-score tracking
7. Heuristic and Deterministic agents

## Project Structure
```text
Skyjo/
├── include/            # Header structure
│   ├── agents/         # Agent header files
│   ├── experiments/    # Experiment header files
│   ├── skyjo/          # Simulation header files
├── src/                # All of the main cpp files
│   ├── agents/         # All of the agent classes
│   ├── experiment/     # All of the experiment code
│   ├── skyjo/          # All of the simulation code
├── tests/              # Includes all of the tests
├── main.cpp            # Project file that runs the scripts
├── CMakeLists.txt      
└── README.md           # This documentation file
```

## Build Instructions
The build includes CLI flags. To run a basic experiment of n games, include the cli flag "-basic" followed by the amount
of runs to do. To run games for a specified amount of time, use the cli flag "-speed". To run a tournament, use the
"-tournament" flag followed with the amount of players per game.