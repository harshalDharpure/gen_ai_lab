CS5205: Advanced Artificial Intelligence Lab - Assignment 1
Assignment Scheduler Program
================================================================

DESCRIPTION:
------------
This program finds all possible valid schedules for completing assignments
with dependencies, given constraints on:
- Number of students (N)
- Prompts per day per student (K)
- Number of days to complete all assignments (m)
- Assignment dependencies (prerequisites)
- Prompt requirements for each assignment

COMPILATION:
------------
To compile the program, use one of the following commands:

For C++ (g++):
  g++ -o assignment_scheduler assignment_scheduler.cpp -std=c++11

For C++ (clang++):
  clang++ -o assignment_scheduler assignment_scheduler.cpp -std=c++11

For Windows (MinGW):
  g++ -o assignment_scheduler.exe assignment_scheduler.cpp -std=c++11

For Windows (Visual Studio):
  cl /EHsc assignment_scheduler.cpp /Fe:assignment_scheduler.exe

EXECUTION:
----------
After compilation, run the program with:

  ./assignment_scheduler <input-filename> <number-of-days>

  OR on Windows:
  assignment_scheduler.exe <input-filename> <number-of-days>

Example:
  ./assignment_scheduler input1.txt 5
  assignment_scheduler.exe input1.txt 5

INPUT FILE FORMAT:
-----------------
The input file should follow this format:

  % Comment lines start with %
  N <number-of-students>
  K <prompts-per-day-per-student>
  A <assignment-id> <prompt-count> <dependency1> <dependency2> ... 0

Notes:
- Comment lines (starting with %) are ignored
- N: Number of students in the group
- K: Maximum prompts each student can use per day
- A: Assignment entry
  - assignment-id: Unique identifier for the assignment
  - prompt-count: Number of prompts needed to complete this assignment
  - dependencies: List of assignment IDs that must be completed first
  - 0: Terminates the dependency list

OUTPUT:
-------
The program prints all valid schedules that:
1. Complete all assignments within the specified number of days
2. Respect all dependency constraints
3. Respect prompt limits per student per day
4. Complete each assignment in a single day (no partial completion)

Each schedule shows which assignments are completed on each day.

SAMPLE INPUT FILES:
-------------------
Three sample input files are provided:
- input1.txt: 12 assignments with sequential and branching dependencies
- input2.txt: 14 assignments with parallel dependency chains
- input3.txt: 15 assignments with complex tree-like dependencies

ALGORITHM:
----------
The program uses a backtracking algorithm to explore all possible schedules:
1. Builds a dependency graph from input
2. For each day, finds all assignments that can be scheduled (dependencies met)
3. Tries all valid combinations of assignments that fit within prompt limits
4. Recursively explores all possibilities
5. Collects all valid schedules that complete all assignments in m days

CONSTRAINTS:
------------
- Each assignment must be completed in a single day
- A student cannot share prompts with another student
- An assignment can only be started after all its dependencies are completed
- Multiple assignments can be done in one day if prompt limits allow
- All assignments must be completed within m days

AUTHOR:
-------
Created for CS5205 Assignment 1
