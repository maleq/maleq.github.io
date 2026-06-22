# kLP

## Compile

```bash
make
```

```bash
make clean
```

## Run

```bash
./klp <graph-file> <k> <initial-confidence> <output-file>
```

Available initial-confidence functions:

- `uniform`
- `degree`
- `avg-jaccard`
- `clustering`

Example:

```bash
./klp input_graph_file 3 avg-jaccard output_communities_file
```

The output file contains one `vertex community_label` pair per line.



### Graph Format

The input is an undirected graph stored as a sorted adjacency list:

```text
num_vertices num_edges
vertex degree
neighbor_1
neighbor_2
...
```
Edges are stored in both directions. Neighbor IDs are sorted from lowest to
highest.

Example graph with four vertices and three edges:

```text
4 3
0 2
1
2
1 2
0
3
2 1
0
3 1
1
```
