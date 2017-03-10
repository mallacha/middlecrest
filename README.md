## Introduction

Middlecrest (provisional name) is a roguelike that will feature a world with procedurally generated political systems, factions, and events that provides a player with an endless number of situations they may encounter. The world of Middlecrest will also incorporate discoverable in-world curated content to immerse the player and perhaps, in the future, generated content. Much of the inspiration of Middlecrest are planned features that never made it into Daggerfall.

## Development
### Tools
#### SQLite

SQLite powers the game. Complete information resides at tools/sqlite but here's a short synopsis:

For Linux/MacOS/*nix/etc.
`$ cd tools/sqlite`
`tools/sqlite$ make`

For Windows:
Open up sqlite.cbp and compile.

#### Cppcheck

Cppcheck is used to analyze everything from array overruns, memory leaks, resource leaks, invalid use of STL and more. It's an invaluable tool to make the code as perfect and correct as possible. To build:

For Linux/MacOS/*nix/etc.
`$ cd tools/cppcheck`
Unzip cppcheck and navigate into it
`tools/cppcheck/cppcheck$ make`

For Windows
Open up the sqlite.cbp and compile
