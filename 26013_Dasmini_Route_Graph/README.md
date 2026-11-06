# Route Graph

A command-line program in C that models a network of cities connected by roads. You can build the map, find the shortest route between two cities, and save or load the network from a file.

## Features

- Add and remove cities
- Add and remove roads (two-way, with a distance)
- Display all cities and their roads
- Find the shortest route between two cities (Dijkstra's algorithm), showing the path and total distance
- Save the network to a file and load it automatically on the next run
- Input validation (letters-only city names, positive distances, no duplicate cities or roads)

## How it works

- **Graph:** a linked list of cities, where each city has its own linked list of roads (an adjacency list).
- **Shortest path:** Dijkstra's algorithm. Each city stores its best known distance, a visited flag, and the previous city on the path. The path is printed recursively from source to destination.
- **Storage:** a plain text file, `routes.txt`.

## Project files

| File | Purpose |
|---|---|
| `graph.h` / `graph.c` | City and road structures, add/remove operations, shortest path |
| `storage.h` / `storage.c` | Save and load the network |
| `validations.h` / `validations.c` | Validation checks |
| `utils.h` | Macros used in program |
| `main.c` | Menu and program loop |


## How to compile and run
```bash
make
./a.out
```
### To remove the build files:

```bash
make clean
```
## How to use

1. Run the program. Any saved map in `routes.txt` is loaded automatically.
2. Pick an option from the menu.
3. To build a map: add cities first, then add roads between them.
4. To find a route: choose the shortest path option and enter the source and destination city names.
5. Saved automatically on exit, so your map is kept for the next run.

## Example

Map: A-B = 4, A-C = 1, C-B = 2

```
Shortest path from A to B
Path: A -> C -> B
Total distance: 3.00
```

## Save file format

```
3
A 2  B 4 C 1
B 2  A 4 C 2
C 2  A 1 B 2
```

- Line 1: number of cities
- Each following line: city name, number of roads, then a `destination distance` pair for each road

## Notes

- City names may contain letters only (no spaces or digits).
- Distances must be greater than zero.