# Overview
This project is a high performance C++ Skyjo Card game simulator.
The card game Skyjo revolves around players getting a hand of twelve
faced down cards that are slowly revealed and exchanged throughout the match.
The goal is for the total score of this hand to be minimized.
The main purpose of this simulator is to test the various agents that have been created
and to use game theory to find an optimal strategy. 
Currently, there are three main agents for testing: random, basic logic, and a heuristic minimax.

## Features
1. A multithreaded simulation that leverages the speed of c++ to test advanced agents on.
2. Independent structure in which the states and their corresponding player states are independent of other states.
3. Selective variable tracking, to only track variables needed with the specific agent used.
4. Reproducible experiment runner
5. Random and rule-based agents
6. Win-rate and average-score tracking
7. Heuristic agents
8. Learning agents

## Project Structure
```text
Skyjo/
├── agents/             # The various agents to test
├── include/            # Header structure
│   ├── agents/         # Agent header files
├── src/                # All of the main cpp files
├── tests/              # Project scripts
├── main.cpp            # Project file that runs the scripts
├── CMakeLists.txt      
└── README.md           # This documentation file
```

## Architecture
```text
Game: Contains
State:
Agents:
Test:
Cli:
```
## Build Instructions

## Agents

## Read More
If you want to understand more about this project,
read my paper on Arxiv:
(link)