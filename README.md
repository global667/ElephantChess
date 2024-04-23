
# ElephantChess


![A Screenshot](Screenshot.png)
##### Table of Contents 
[Introduction](#Introduction) 

[Rules](#Rules)

[Installation](#installation)  

[Usage](#usage)    

[Issues](#issues) 

<a name="headers"/>

## Introduction

Chinese chess GUI with weakbot and in pre alpha stadium...

The aim is to play and analyse games for Chinese Chess like with "Arena" 
for Europe Chess (with the commercial "Fritz" in mind).

- Chinese Chess on Wikipedia: <a href="https://en.wikipedia.org/wiki/Xiangqi">XiangQi</a>
- Europe Chess on Wikipedia: <a href="https://en.wikipedia.org/wiki/Chess">Chess</a>

## Rules
Xiangqi, also known as Chinese Chess, is played on a 9x10 board divided by a "river" with pieces positioned on the intersections (points). The game features two players, Red and Black, each starting with one General, two Advisors, two Elephants, two Horses, two Chariots, two Cannons, and five Soldiers.

Objective: The main goal is to checkmate the opponent's General.

### Gameplay Rules:

Movement: Each type of piece has specific movement rules:
- General: Moves one point horizontally or vertically within the palace (a 3x3 area in the central columns of each player’s home territory).
- Advisor: Moves one point diagonally, also confined within the palace.
- Elephant: Moves exactly two points diagonally and cannot cross the river.
- Horse: Moves one point orthogonally followed by one point diagonally outward, similar to an “L” shape. Its movement can be blocked by an intervening piece.
- Chariot: Moves any number of points horizontally or vertically across the board.
- Cannon: Moves like the Chariot but captures by jumping over exactly one intermediate piece to take an opponent's piece.
- Soldier: Moves and captures one point forward vertically. Upon crossing the river, it can also move and capture horizontally.
- Capturing: A piece captures an opponent's piece by moving into the point occupied by that piece, according to its movement abilities, except for the Cannon, which must jump to capture.
- General's Safety: The two Generals must never face each other directly on an open file (column with no intervening pieces).
- Check and Checkmate: A player's turn ends in check if their General could be captured on the opponent’s next move. The game ends in checkmate if a player’s General is in check and no legal move can remove or block the threat.
Players alternate turns, moving one piece per turn, aiming to maneuver into a position to checkmate the opposing General. The game can also end in a stalemate or draw if a player has no legal move and their General is not in check.

## Installation
### Use QtCreator
- Go to https://www.qt.io/download-open-source and login
- Download Qt6.x for Linux with "Qt Online Installer"
- Import repo in QtCreator or use

    ```git clone https://github.com/global667/ElephantChess.git```

- Run `CMakeLists.txt` on QtCreator

### Use installation package
- go to www.elephant-chess.com download the zip file extract it and run installation

## Issues
- Should run on Windows and macOS too
- Tested on Ubuntu/wsl
- Have fun!

