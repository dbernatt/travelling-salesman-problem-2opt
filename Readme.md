# Travelling Salesman Problem (TSP) with 2-opt.

The **travelling salesman problem** asks the following question: "Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city?". It is an [NP-hard](https://en.wikipedia.org/wiki/NP-hardness "NP-hardness") problem in [combinatorial optimization](https://en.wikipedia.org/wiki/Combinatorial_optimization "Combinatorial optimization"), important in [theoretical computer science](https://en.wikipedia.org/wiki/Theoretical_computer_science "Theoretical computer science") and [operations research](https://en.wikipedia.org/wiki/Operations_research "Operations research").

## 2-opt algorithm

In **2-opt** is a simple local search algorithm for solving the [traveling salesman problem](https://en.wikipedia.org/wiki/Traveling_salesman_problem "Traveling salesman problem"). The 2-opt algorithm was first proposed by Croes in 1958, although the basic move had already been suggested by Flood. The main idea behind it is to take a route that crosses over itself and reorder it so that it does not.

![](gifs/2_opt_example.gif)

## In action

Operating system: **Ubuntu 20.04.3**

1. Install **Sfml** dependency.
   `$ sudo apt-get install libsfml-dev`

2. Compile.
   `$ make`

3. Start.
   `$ ./TSP`

4. Cleanup :).
   `$ make clean`
