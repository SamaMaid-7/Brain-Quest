BRAIN QUEST — CODE OVERVIEW
===========================

AT A GLANCE
-----------
Project name:       Brain Quest
Main source file:   main.cpp
Source length:      402 lines
Programming language: C++
Graphics library:   WinBGIm / BGI (graphics.h)
Platform:           Windows desktop application
Input:              Mouse and keyboard
Game structure:     7 mini-games with progressive level unlocking
Code style:         Functions, a struct, arrays, loops, conditions, and variables

LIBRARIES / HEADERS USED
------------------------
graphics.h   - Drawing shapes, text, buttons, and the game interface.
winbgim.h    - WinBGIm support, including graphics-window and mouse features.
conio.h      - Keyboard input functions such as kbhit() and getch().
windows.h    - Windows-specific window functions, including maximize support.
iostream     - Standard input/output streams.
cstdlib      - General utilities, including random-number utilities.
ctime        - Time-related functions, used to seed random behaviour.
cstring      - C-style string utilities.
string       - The C++ string type.

MAIN DATA STRUCTURES AND VARIABLES
----------------------------------
Button struct
    Stores a button's position, size, label, fill colour, border colour,
    and text colour.

unlockedLevel
    Tracks the highest level unlocked during the current run.

totalScore
    Tracks the score accumulated during the current run.

quitGame
    Indicates when the user chooses to exit.

IMPORTANT HELPER FUNCTIONS
--------------------------
textAt()
    Draws text at a specified position.

centeredText()
    Draws horizontally centred text.

drawButton()
    Draws a button and its label.

inside()
    Checks whether a coordinate falls inside a button.

clickAt()
    Checks for a mouse click and reads its position.

waitKeyOrClick()
    Waits for keyboard input or a mouse click.

clearScreen()
    Clears the graphics screen and draws the screen heading.

messageScreen()
    Displays a result or status message.

pauseForContinue()
    Shows a message and waits for the player to continue.

THE SEVEN GAMES — SHORT EXPLANATIONS
------------------------------------

LEVEL 1 — NUMBER GUESSING (Easy)
The computer chooses a number from 1 to 100. The player has a limited
number of attempts and receives a hint saying whether the target is
higher or lower after a wrong guess.

LEVEL 2 — MEMORY MATCH (Easy)
The player reveals two cards at a time and searches for matching pairs.
The goal is to find all pairs before reaching the move limit.

LEVEL 3 — WORD SCRAMBLE (Easy)
The letters of a word are shuffled. The player types the correct word.
The level contains five words and allows limited attempts.

LEVEL 4 — TIC-TAC-TOE (Easy–Medium)
The player competes against the computer on a 3×3 grid. The aim is to
make a row, column, or diagonal of three matching symbols.

LEVEL 5 — CODE BREAKER (Medium)
The player tries to discover a hidden sequence of four coloured pegs.
Feedback helps identify colours that are correct and/or in the correct
positions. The code must be found within the allowed guesses.

LEVEL 6 — LIGHTS OUT PUZZLE (Medium)
Selecting a square toggles that square and its neighbouring lights.
The goal is to switch all lights off within the move limit.

LEVEL 7 — SUDOKU CHALLENGE (Final level)
The player fills the empty cells in a 9×9 Sudoku grid. Every row,
column, and 3×3 box must contain the digits 1–9 without repetition.

HOW THE GAME FLOW WORKS
-----------------------
1. The main menu displays the available levels.
2. Level 1 is unlocked at the start.
3. The player chooses an unlocked level using the mouse or keyboard.
4. The selected game's function runs and checks whether the player wins.
5. Winning updates the score and unlocks the next level.
6. The player returns to the menu to continue.
7. Progress and score are held in memory and reset when the program closes.

OOP / PROGRAMMING CONCEPTS TO MENTION
-------------------------------------
- struct: Button groups related button information into one type.
- Functions: Each game and reusable interface task is separated into a
  function, making the program easier to read and maintain.
- Arrays: Used where a game needs a collection of values, such as boards
  or cards.
- Loops: Repeat turns, input checks, and board operations.
- Conditions: Check guesses, matching pairs, winning states, and level
  unlocking.
- Variables: Keep track of choices, attempts, moves, score, and progress.
- Randomness: Used for games that need a random number, card arrangement,
  or secret code.

NOTES
-----
This is a Windows desktop program, not a browser website. The graphical
interface is drawn by WinBGIm using graphics.h. The code requests a
maximized graphics window, but the exact window behaviour can depend on
the installed WinBGIm version and the computer's display settings.

Line count refers to the current main.cpp file and will change if the
source code is edited.
