BRAIN QUEST - C++ OOP AND COMPUTER GRAPHICS
===========================================

This is a beginner-level C++ project with seven small brain-game challenges.
It uses OOP classes and the WinBGIm graphics.h library to draw a window, text,
lines, rectangles and a circle. There is no database.

IMPORTANT ABOUT LOCALHOST
-------------------------
A C++ graphics.h program is a desktop application. VS Code Live Server only serves
HTML/CSS/JavaScript files; it does not compile or run C++ graphics programs on localhost.
Because your course requires C++ OOP and computer graphics, this project runs as a
graphics window, not as a website. This is the simplest honest setup for graphics.h.

REQUIREMENTS
------------
- Windows
- Code::Blocks or another C++ compiler configured for WinBGIm
- WinBGIm graphics.h library and its BGI linker library

SETUP
-----
1. Extract this ZIP into a folder.
2. Install Code::Blocks with MinGW, if not already installed.
3. Configure WinBGIm (graphics.h and libbgi) for your compiler.
4. Create/open a C++ Console project and add main.cpp.
5. Add these linker options if required by your WinBGIm package:
   -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
6. Build and Run.

VS CODE
-------
You can open the extracted folder in VS Code to read/edit the source code. However,
VS Code alone does not provide a C++ compiler or graphics.h. You must configure the
compiler and WinBGIm library. The exact steps vary by installation.

GAMES
-----
1. Number Guess
2. Memory Match
3. Word Scramble
4. Tic-Tac-Toe mini puzzle
5. Code Breaker
6. Lights Out mini puzzle
7. Sudoku mini row puzzle

The games are deliberately small practice challenges, not full commercial games.
The unlock state is held in memory and resets when the program closes.

OOP CONCEPTS
------------
- Class: Game and the seven game classes.
- Object: game1, game2, etc. created in main().
- Inheritance: each game class inherits from Game.
- Abstraction: Game defines the common play() function.
- Polymorphism: a Game pointer calls the correct derived class's play() function.
- Encapsulation: each game's play logic is kept inside its class.

COMPUTER GRAPHICS CONCEPTS
--------------------------
- initwindow() creates the graphics window.
- setcolor() selects a drawing colour.
- line() draws lines.
- rectangle() draws rectangles and the game boards.
- outtextxy() displays text at coordinates.
- fillellipse() draws the light in Lights Out.
- cleardevice() clears the screen.
- closegraph() closes the graphics window.

APPROXIMATE LINE COUNT
----------------------
main.cpp is roughly 220-250 lines depending on whether blank lines and comments
are counted. Your editor's line counter gives the exact count for your copy.
