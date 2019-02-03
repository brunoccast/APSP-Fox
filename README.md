# APSP-Fox

Parallel program (MPI) that uses Repeated Squaring combined with
Fox's algorithm to solve instances of the All-Pairs Shortest Path
problem.

## Execution

1. `$ cd src/ && make`
2. `$ mpirun -np x --hostfile y fox`
  + replace x and y accordingly

Input consist in an integer **N** representing the number of nodes
in the graph, followed by NxN doubles each representing links, and
costs, between pairs of nodes of in the graph. E.g:

```
6
0 2 0 5 0 0
0 0 0 0 0 0
0 2 0 0 0 5
0 0 0 0 1 0
3 9 3 0 0 0
0 0 0 0 1 0
```

The corresponding output should be:

```
0 2 9 5 6 14
0 0 0 0 0 0
9 2 0 14 6 5
4 6 4 0 1 9
3 5 3 8 0 8
4 6 4 9 1 0
```

Run `$ make clean` to, optionally, remove the executable.

## Tests

1. `$ cd test/ && make`
2. `$ ./exec`
  + exec is either `fox`, `mem` or `util`

Tests doesn't cover the totality of the program, I made them as
a first introduction to C++ testing.

## References

+ [Repeated Squaring](http://users.cecs.anu.edu.au/~Alistair.Rendell/Teaching/apac_comp3600/module4/all_pairs_shortest_paths.xhtml)
+ [Fox example](http://www.lac.inpe.br/~stephan/CAP-372/Fox_example.pdf)
+ [A User's Guide to MPI](https://www.researchgate.net/publication/2487381_A_User's_Guide_to_MPI)