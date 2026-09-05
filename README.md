# Pokedex Management System (C)

A modular command-line Pokedex system implemented in C, showcasing clean architecture, dynamic data structures, and rigorous memory management without leaks.

## Overview
The application parses a structured configuration file to initialize a catalog of Pokémon entities, elemental types, and their combat attack effectiveness relationships (effective-against-me / effective-against-other). Users interact with the Pokedex via an interactive CLI menu to query entities and mutate relationship graphs dynamically.

## Key Technical Highlights
- Dynamic Memory Management: Uses strict manual allocation (malloc, realloc, free) sizing dynamic arrays precisely to the required capacity with zero memory leaks (verified with Valgrind).
- Encapsulated Architecture: Distinct separation of concerns across header and source files (Defs.h, Pokemon.h, Pokemon.c, and Pokedex.c).
- File Parsing: Robust file reading and parsing of multi-line configuration schemas.
- Defensive Programming: Validates memory allocation results and pointer validity across all operations.

## File Structure
- Pokemon.h / Pokemon.c: Type definitions (structs) and core ADT operations for Pokémon, biological specs, and elemental types.
- Pokedex.c: Main entry point handling CLI interactions and configuration parsing.
- Defs.h: Global status codes and boolean definitions.

## Compilation
gcc -Wall -Wextra -std=c99 Defs.h Pokemon.h Pokemon.c Pokedex.c -o Pokedex

## Run Application
./Pokedex <numberOfTypes> <numberOfPokemons> <configurationFile>

Example:
./Pokedex 4 5 configuration.txt

## Valgrind Leak Check
valgrind --leak-check=full --track-origins=yes ./Pokedex 4 5 configuration.txt
