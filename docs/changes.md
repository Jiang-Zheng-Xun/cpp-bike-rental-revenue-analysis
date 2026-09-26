# Investigation and Change Record

This record separates the original course implementation, AI assisted findings,
reviewed changes, and observed results. It does not claim that the Part 2
heuristic achieves a global revenue maximum.

## Sources and scope

The original program was a single C++11 source file written for a 2021 data
structures course. The course specification, illustrations, provided test cases,
original report, and student-ID-named files are kept outside this repository.
The original source and its initial outputs are preserved locally.

The repository contains a reviewed working version and separately authored
examples. The provided open test cases were used locally for regression checks;
their input files are not included here.

## Original behavior

Part 1 accepts a rental when the requested bike type is available at the
station and otherwise rejects it. It updates the inventory on rentals and
returns, and charges according to the shortest travel time and the applicable
rate.

The original Part 2 heuristic prepositions bikes at stations with a shortage,
offers another bike type at a reduced rate when necessary, and uses Part 1
outputs if its calculated revenue is lower. It is a heuristic, not a proven
optimization algorithm.

## Reviewed changes

| Finding and reproduction | Change | Verification |
|---|---|---|
| On all three provided open cases, Part 2 status omitted every `road:` heading. Transfers in two cases began with `transfer2` instead of `transfer 2`. | Wrote the Part 2 road heading to its correct output stream and added the missing transfer separator. | Reran all three cases; Part 1 output was unchanged. Part 2 contained 6, 9, and 9 road headings respectively. |
| A bike transferred over a 10 minute edge at time 0 could be rented at the destination at time 1. Sorting the distance array also overwrote distances needed for transfer decisions. | Kept the original distances separate from their sorted copy and held transfers pending until arrival. | In a small case, a request at time 9 was rejected; at time 10, the first of two requests was accepted and the second rejected. |
| A substituted rental produced both `discount electric` and `accept`. For a regular electric rate of 7, reduction factor of 0.8, and three rental minutes, the original calculation produced 17. | Produced one response per rental and rounded the reduced per minute rate before multiplying by duration. | The separately authored `discount_rounding` example produces `discount electric` and revenue 18. |
| A transfer can cost more than the additional rental revenue. | Retained the existing fallback to Part 1 when Part 2 revenue is lower. | In a small low revenue case, both Part 2 output files matched their Part 1 counterparts byte for byte. |

These changes were proposed and examined with AI assistance. I reviewed the
specification and observed behavior before applying them, kept the original
source separately, and reran targeted cases and regression inputs.

## Results on provided open cases

| Case | Part 1 revenue | Revised Part 2 revenue | Difference |
|---|---:|---:|---:|
| open_basic1 | 24,210 | 24,210 | 0 |
| open_basic2 | 375,995 | 393,356 | +17,361 |
| open_basic3 | 1,680,165 | 1,701,461 | +21,296 |

For these three cases, the output retained every input event, provided one
response immediately after each rental, and listed the same total number of
bikes before and after processing. These checks do not establish correctness
on hidden cases or prove that the revenue is maximal.

## Reproduce the included example

From the repository root, build the program as described in `README.md`,
then run:

```bash
cd examples/discount_rounding
../../build/bike_rental
```

Expected Part 1: `reject`, revenue `0`.

Expected Part 2: `discount electric`, revenue `18`.

The program writes four output files in the example directory. They are
generated artifacts and are ignored by Git.

## Known limits and follow-up work

The program still relies on fixed capacities and assumes well-formed input.
Input validation, malformed data, disconnected stations, large data sets, and
all possible transfer and return schedules have not been fully validated.
An independent event and revenue oracle, additional boundary cases, and
performance measurements would make the evidence stronger.

The current Part 2 policy makes local transfer and substitution decisions.
A higher result on the provided cases does not imply an optimal result.
Further analysis may compare alternative graph representations, event
processing designs, and demand-aware transfer policies.
