#  Passepartout

[![official JetBrains project](http://jb.gg/badges/official.svg)](https://confluence.jetbrains.com/display/ALL/JetBrains+on+GitHub)
[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/Naereen/StrapDown.js/blob/master/LICENSE)

> ***Passepartout***  is a fictional character in [Jules Verne](https://en.wikipedia.org/wiki/Jules_Verne)'s novel [*Le tour du monde en quatre-vingts jours*]( https://en.wikipedia.org/wiki/Around_the_World_in_Eighty_Days), and I named this repo his name. This repo is some attempts to solve TSP in different ways.

### src

I calculated the optimal path, optimal cost and run time.

- DynamicProgramming_48.c
  -  Dynamic programming.
  -  Time Complexity: `O(2^n*n^2)`
  -  Exact solution.
- Approximation.c
  - Approximation algorithms.
  - Time Complexity: `O(n^3)`
  - Inexact solution ( smaller than 2 * optimal cost);
- SimulateAnneal_48.c
  -  Simulated annealing algorithm.
  -  Exact solution if you run long enough.

![](https://github.com/VenciFreeman/Passepartout/blob/master/fig/sa1.jpg)

- SimulateAnneal_358.c
  -  Simulated annealing algorithm, too.
  -  Exact solution if you run long enough.

![](https://github.com/VenciFreeman/Passepartout/blob/master/fig/sa2.jpg)

### data

- att7.txt
  - The number of cities is 7. Directed graph, given distance directly.
- att20.txt
  - The number of cities is 20. Directed graph, given distance directly.
- bays29.txt
  - The number of cities is 20. Directed graph, given distance directly.
- us48.dat
  - The number of cities is 48. Undirected graph, given the coordinates of each city.
- att358.dat
  - The number of cities is 358. Directed graph.

### CMAKELists

- Set a large stack space. By the way, I'm using CLion provided by Jet Brains.
