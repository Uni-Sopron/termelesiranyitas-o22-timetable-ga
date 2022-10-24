# Timetable

[![build](https://github.com/Uni-Sopron/termelesiranyitas-o22-timetable-ga/actions/workflows/build.yml/badge.svg)](https://github.com/Uni-Sopron/termelesiranyitas-o22-timetable-ga/actions/workflows/build.yml)

Timetable is a small, dependency-free CLI tool responsible for scheduling timetables using a GA (genetic algorithm) approach.

It can handle subjects, lecturers, groups, and rooms.

## Installation

    make
    make install

## Usage

Consult with the `config.ini` to meet your requirements, and then execute the binary in the same directory with the configuration file.

After each iteration, the program will print out the average fitness score of the currently processed population.

After satisfying the chosen exit model, two folders (`groups`, `lecturers`) are generated, containing the final timetable from each participant viewpoint.

## Configuration

| Parameter                | Description                                                                           |
| ------------------------ | ------------------------------------------------------------------------------------- |
| daily_starting_hour      | The minimum possible time for a subject to start (hh::mm)                             |
| daily_ending_hour        | The maximum possible time for a subject to start (hh:mm)                              |
| break_time               | Break time in minutes after each lecture                                              |
| lecture_duration         | Lecture time in minutes                                                               |
| mutation_percent         | The percentage for each timetable to mutate                                           |
| initial_population_size  | The size of the starting population                                                   |
| seed                     | The seed used for the random number generator                                         |
| max_age                  | Maximum age for a timetable inside the pool (used with the aging rotation model)      |
| max_generation_count     | Maximum generation count (used with the max generation exit model)                    |
| no_convergence_tolerance | Maximum number of consecutive iterations without improvement (convergence exit model) |

## Currently implemented operators

### Mutation

- **Swap mutate:**
  Swaps a single subject's time schedule with a random available date. This method uses dates that are might not be assigned to other subjects (not part of the original genome).

- **Rotate mutate:**
  Swaps multiple subject's schedule within a randomized sub range of the timetable. Only uses dates that are part of the sub range.

### Selection

- **Tournament selection:**
  Selects two timetables randomly from the individual pool. Returns the better one (with lower fitness score).

- **Elitist selection:**
  Chooses one of the best individuals from the pool. Generally requires higher initial population number for better results.

### Crossover

- **One-point crossover:**
  One range for the new timetable's genome will be selected strictly from one parent, and another range from the other parent.

- **Uniform crossover:**
  Each gene (date, room) of the newborn timetable will be selected randomly from the two parents.

### Rotation

- **Same count best fit rotation:**
  The pool of timetables will be sorted based on their fitness level, and the best `initial_population_size` (specified in `config.ini`) will be selected, the rest will be discarded.

- **Aging rotation:**
  The program keeps track of all the individual with their occurrence, and if a specific timetable occurs more than `max_age` (variable specified in `config.ini`), it will be discarded.

## Exit strategies:

- **Found Strategy:**
  The pool will be improving until a perfect (with non-overlapping subjects and rooms) timetable is found.

- **Convergence strategy:**
  The program will terminate if a predefined number of consecutive iterations (`no_convergence_tolerance` in the configuration file) won't generate a pool with a better average of fitness values.

- **Max population strategy:**
  Exit after `max_generation_count` iterations.

## Early exit

An error is thrown right after parsing the configuration file, if too many subjects are given to a lecturer or a group (more than the available dates).
