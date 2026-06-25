# CPU & Cache Simulator

This is a C++ program that simulates how a CPU schedules tasks and fetches memory from different cache levels (L1, L2, L3, and RAM). 

## Features
* **Scheduler:** Round Robin (Quantum = 3)
* **Cache Policy:** LRU (Least Recently Used) - *Bonus Feature*
* **Input:** Reads task data (burst times and memory blocks) from `input_task2.txt`

## Scheduler Justification
I chose the Round Robin algorithm with a time quantum of 3 because it keeps CPU execution fair. Since the tasks in the input file have completely different burst times, Round Robin ensures that the shorter tasks don't get stuck waiting forever behind the massive ones, preventing starvation.

## How to Run
1. Make sure both `solution.cpp` and `input_task2.txt` are in the same folder.
2. Compile the code using terminal:
   `g++ solution.cpp -o solution`
3. Run the executable:
   `./solution`
