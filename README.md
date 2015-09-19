# Patterns_in_Game_of_Life
A series of programs tracking the evolution and termination of various shapes in Conway's Game of Life

This project is centered around simulating a cellular automaton according the rules of Conway's Game of Life.
They were made for a research project that involved studying patterns produced in the Game of Life.

The file ```Iterations_GameofLife.cpp``` is the primary file. This allows for the creation of a cellular automaton configured
initially as a solid square of a user-defined size. All other programs are variations of this one.

The file ```Iterations_GameofLife_Display.cpp``` is made to output the cellular automaton after each generation.
The file ```Iterations_GameofLife_square.cpp``` generates a cellular automaton configured initially as a hollow square.
```ProbabilityModelSimulator.cpp``` implements a probabilistic model to predict the behavior of a cellular automaton in the Game of Life.
