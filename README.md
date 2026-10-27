# akinator_project

A console Akinator-style guessing game written in C++23. The player thinks of a character,
the game asks yes/no questions and tries to guess it. If the guess is
wrong, the game learns a new character, so the knowledge base grows over time.

Team project (4 people), HSE University, Faculty of Computer Science. Grade: 9/10.

## Features
- Question tree: the game walks a binary tree of yes/no questions down to an answer
- Learning: after a wrong guess the player can teach the game a new character
- Player profiles: name and statistics (games played / won / lost) saved to a file
- Saving and loading the knowledge tree as JSON
- Performance benchmarks for file loading and entity search
- 23 unit tests (GoogleTest)

## Architecture

| Component | Responsibility |
|---|---|
| `Node`, `QuestionNode`, `AnswerNode` | Tree nodes (OOP: inheritance and polymorphism, `std::unique_ptr` ownership) |
| `KnowledgeTree` | Tree navigation and the learning algorithm |
| `GameSession` | Game flow of one session |
| `Player` | Player data and statistics, save/load |
| `TreeSerializer` | Tree <-> JSON conversion (nlohmann/json) |
| `FileHandler` | Reading and writing files |
| `EntitySearcher` | Searching objects in the tree and paths to them |
| `Benchmark` | Measuring file loading and search speed |

## Tech stack
C++23, CMake (3.20+), nlohmann/json 3.11.3, GoogleTest 1.14.0 (both fetched
automatically via CMake `FetchContent`), Git/GitHub (feature branches, pull
requests, code reviews).


## Team
- **Angelina Radaeva** — knowledge tree: `Node`, `QuestionNode`, `AnswerNode`,
  `KnowledgeTree`; tree navigation; learning algorithm
- **Sofia Servetnik** — game logic (`GameSession`), player data and statistics
  (`Player`), tests
- **Anastasia Ivanova** — tree traversal and search (`EntitySearcher`),
  performance checks (`Benchmark`), tests
- **Maya Izotova** — storage: JSON serialization (`TreeSerializer`) and file
  handling (`FileHandler`)
