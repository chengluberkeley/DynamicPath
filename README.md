# Dynamic Path

This repo implements the dynamic path data structure based on [**Sleator, D.D. and Tarjan, R.E.** A data structure for dynamic trees. *J. Comput. Sys. Sci.*, 24: 362-391, 1983.](https://www.cs.cmu.edu/~sleator/papers/dynamic-trees.pdf), with adaptation by [**Hochbaum, D.S. and Lu, C.** A faster algorithm solving a generalization of isotonic median regression and a class of fused lasso problems. *SIAM J. Opt.*, 27(4): 2563-2596, 2017.](https://hochbaum.ieor.berkeley.edu/html/pub/Isotonic-HLuSIAM-Opt2017.pdf)

The data structure maintains a collection of vertex-disjoint paths, where each edge has a real-valued cost. The following operations can be computed efficiently by the dynamic path data structure:
- `p := path(v)`: Return the path `p` containing vertex `v`.
- `v := head(p)`: Return the head vertex `v` of path `p`.
- `v := tail(p)`: Return the tail vertex `v` of path `p`.
- `u := before(v)`: Return the vertex `u` before vertex `v` on `path(v)`. If `v` is the head of the path, return `NIL`.
- `u := after(v)`: Return the vertex `u` after vertex `v` on `path(v)`. If `v` is the tail of the path, return `NIL`.
- `x := pcost_before(v)`: Return the real-valued cost `x` of the edge `(before(v), v)`. If vertex `v` is the head of the path, return `NaN`.
- `x := pcost_after(v)`: Return the real-valued cost `x` of the edge `(v, after(v))`. If vertex `v` is the tail of the path, return `NaN`.
- `v := pmincost_before(p)`: Return the vertex `v` closest to `head(p)` such that `(before(v), v)` has the minimum cost among edges on path `p`. If `p` contains only one vertex (degenerated case), return `NIL`.
- `v := pmincost_after(p)`: Return the vertex `v` closest to `tail(p)` such that `(v, after(v))` has the minimum cost among edges on path `p`. If `p` contains only one vertex (degenerated case), return `NIL`.
- `pupdate(p, x)`: Add real value `x` to the cost of every edge on path `p`.
- `p3 := concatenate(p1, p2, x)`: Concatenate paths `p1` and `p2` by adding the edge `(tail(p1), head(p2))` of real-valued cost `x`. Return the merged path `p3`.
- `[p1, p2, x] := split-before(v)`: Split `path(v)` into (up to) two parts by deleting the edge `(before(v), v)`. Return a list `[p1, p2, x]`, where `p1` is the subpath consisting of all vertices from `head(path(v))` to `before(v)`, `p2` is the subpath consisting of all vertices from `v` to `tail(path(v))`, `x` is the cost of the deleted edge `(before(v), v)`. If `v` is originally the head of `path(v)`, `p1` is `NIL` and `x` is `NaN`.
- `[p1, p2, y] := split-after(v)`: Split `path(v)` into (up to) two parts by deleting the edge `(v, after(v))`. Return a list `[p1, p2, y]`, where `p1` is the subpath consisting of all vertices from `head(path(v))` to `v`, `p2` is the subpath consisting of all vertices from `after(v)` to `tail(path(v))`, `y` is the cost of the deleted edge `(v, after(v))`. If `v` is originally the tail of `path(v)`, `p2` is `NIL` and `y` is `NaN`.

For a collection of dynamic paths with a total of $O(n)$ vertices, the above operations have the following complexities:
- `p := path(v)`: $O(\log n)$
- `v := head(p)`: $O(1)$
- `v := tail(p)`: $O(1)$
- `u := before(v)`: $O(\log n)$
- `u := after(v)`: $O(\log n)$
- `x := pcost_before(v)`: $O(\log n)$
- `x := pcost_after(v)`: $O(\log n)$
- `v := pmincost_before(p)`: $O(\log n)$
- `v := pmincost_after(p)`: $O(\log n)$
- `pupdate(p, x)`: $O(1)$
- `p3 := concatenate(p1, p2, x)`: $O(\log n)$
- `[p1, p2, x] := split-before(v)`: $O(\log n)$
- `[p1, p2, y] := split-after(v)`: $O(\log n)$

## Build from the source
This project is a `cmake` project. To build from the source:
```
mkdir build && cd build
cmake .. && make -j5
```

It builds a static library `libdynamic_path.a` under the directory `lib/`, and an executable for testing arrays under the directory `bin/`.
