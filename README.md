# Bike Rental Revenue Analysis — C++ Data Structures Portfolio

> Draft README. Repository contents and publication scope are still under review.

## Background

This project began as my final project for a 2021 data structures course. I am revisiting my original C++ program to preserve its history, make its build and tests reproducible, and document how I used AI to help trace code, design boundary cases, and review changes. I evaluate proposed changes against the assignment specification and observed results before adopting them.

The program reads an undirected station map, initial bike inventories, rental rates, and time ordered rental and return events. It produces responses, final inventories, and revenue for a basic policy (Part 1) and an advanced policy (Part 2).

## Original design and policies

- **Graph and shortest paths:** An adjacency matrix represents the undirected station map. A custom implementation of Dijkstra’s algorithm calculates shortest travel times. A rental uses the discounted rate when its duration equals the shortest travel time; otherwise, it uses the regular rate.
- **Inventory:** Each station and bike type has a custom min heap. The bike with the smallest ID is rented first.
- **Part 1:** Accept a request when the requested type is available at that station; otherwise, reject it. Returns update inventory and revenue.
- **Original Part 2:** Attempt to move a bike from a nearby station to a station lacking that type. If the requested type is unavailable, attempt a discounted rental of another type. Use the Part 1 result if the calculated Part 2 revenue is lower.

Part 2 is a **heuristic**. The current evidence does not establish that it maximizes revenue in every scenario.

## Example: when a transferred bike becomes available

Suppose station 1 has no electric bikes, station 2 has two, and the shortest trip between them takes 10 minutes. Part 2 initiates a transfer from station 2 to station 1 at time 0.

| Event | Expected inventory decision |
|---|---|
| Time 9: request an electric bike at station 1 | Reject: the transferred bike is still in transit. |
| Time 10: first request at station 1 | Accept: the bike has arrived. |
| Time 10: second request at station 1 | Reject: requests are processed in input order, and the first renter took the only available bike. |

The original program placed a transferred bike in the destination inventory when it made the transfer decision. That allowed a rental before the bike arrived. The revised version keeps transfers pending and adds a bike to the destination inventory when its arrival time is reached. I checked this boundary case and reran the three provided open test cases in an isolated environment and in my local WSL environment.

## AI assisted investigation and reviewed changes

| Evidence | Original behavior | Reviewed change and verification |
|---|---|---|
| Specification and output comparison | Part 2 status omitted `road:` headings; transfer lines lacked a space after `transfer`. | Corrected the output stream and format. Part 1 output remained unchanged. |
| Shortest path and event time comparison | A transferred bike could be rented before arrival. Sorting distances also altered the original distance data used in transfer decisions. | Preserved the unsorted distances and added bikes to inventory at arrival. Checked times 9 and 10, including two requests at the same time. |
| Small discount example | One substitution produced both `discount <type>` and `accept`. The program rounded the discounted total instead of the per minute rate. | Produced one `discount <type>` response and rounded the discounted per minute rate before multiplying by duration. The example yields 18 rather than the original 17. |
| Low revenue example | Part 2 may need to use the basic policy result. | Confirmed that both Part 2 output files match their Part 1 counterparts when fallback is selected. |

These cases document a review sequence: **specification → AI assisted investigation → my review → minimal reproduction → change → regression check**. Each result should remain linked to its input and output so a reader can inspect the evidence.

## Results on the provided open test cases

| Test case | Part 1 revenue | Revised Part 2 revenue | Difference |
|---|---:|---:|---:|
| open_basic1 | 24,210 | 24,210 | 0 |
| open_basic2 | 375,995 | 393,356 | +17,361 |
| open_basic3 | 1,680,165 | 1,701,461 | +21,296 |

These figures describe **only the current program’s results on three open test cases**. They do not establish a global optimum or correctness on hidden cases. The course provided test cases remain local. A separately authored example would be needed for a publicly reproducible demonstration, subject to review before publication.

## Limitations and future work

Further work includes independent accounting of events, revenue, and bike movements; more valid and boundary inputs; and review of fixed capacities, input error handling, and performance on larger cases. I also plan to analyze the time and space costs of the adjacency matrix, shortest path algorithm, and min heaps, then evaluate alternative graph representations and transfer policies.

The model could inform discussions of shared vehicles, equipment lending, or inventory transfers across locations. These are possible extensions, not features currently implemented.

## Provenance and data scope

I wrote the original program for a data structures course. I keep the course specification, illustrations, test cases, and original report separately. The repository will be private by default. Before changing its visibility, I will review rights to course materials, student ID references, and other personal information. The original program, subsequent changes, and verification evidence will be clearly distinguished.

## Build and run

Requirements: a C++11 compiler on Linux. The current version was built with
g++ 11.4 on Ubuntu 22.04 under WSL. The original course environment specified
Ubuntu 20.04 and C++11.

From the repository root:

```bash
mkdir -p build
g++ -std=c++11 -g -Wall -Wextra -Wpedantic \
  src/bike_rental.cpp -o build/bike_rental
```

The program reads four files from `./test_case/` relative to its current working
directory and writes `part1_response.txt`, `part1_status.txt`,
`part2_response.txt`, and `part2_status.txt` there. To run the reviewed,
self-authored example after it has been added:

```bash
cd examples/discount_rounding
../../build/bike_rental
```

Compare the generated response and final revenue with the expected values
documented for that example. Generated output files are ignored by Git.
