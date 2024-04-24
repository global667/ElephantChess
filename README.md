<div align="center"> 
<code>
 ____     ___                     __                          __        ____     __                                 
/\  _`\  /\_ \                   /\ \                        /\ \__    /\  _`\  /\ \                                
\ \ \L\_\\//\ \       __   _____ \ \ \___       __       ___ \ \ ,_\   \ \ \/\_\\ \ \___       __     ____    ____  
 \ \  _\L  \ \ \    /'__`\/\ '__`\\ \  _ `\   /'__`\   /' _ `\\ \ \/    \ \ \/_/_\ \  _ `\   /'__`\  /',__\  /',__\ 
  \ \ \L\ \ \_\ \_ /\  __/\ \ \L\ \\ \ \ \ \ /\ \L\.\_ /\ \/\ \\ \ \_    \ \ \L\ \\ \ \ \ \ /\  __/ /\__, `\/\__, `\
   \ \____/ /\____\\ \____\\ \ ,__/ \ \_\ \_\\ \__/.\_\\ \_\ \_\\ \__\    \ \____/ \ \_\ \_\\ \____\\/\____/\/\____/
    \/___/  \/____/ \/____/ \ \ \/   \/_/\/_/ \/__/\/_/ \/_/\/_/ \/__/     \/___/   \/_/\/_/ \/____/ \/___/  \/___/ 
                             \ \_\                                                                                  
                              \/_/                                                                                  
</code>
<br>
<br>
   
.::
[![Build ElephantChess](https://github.com/global667/ElephantChess/actions/workflows/build.yml/badge.svg)](https://github.com/global667/ElephantChess/actions/workflows/build.yml)
::.
<div align="center"> 

.::
![](https://badgen.net/static/license/GNU%20GPL%203.0/)
:::
![](https://badgen.net/github/release/global667/ElephantChess/)
::.
<!-- ![](https://badgen.net/github/release/stable/global667/ElephantChess/) -->
</div>

## Table of Contents
- [Introduction](#introduction)
- [Rules](#rules)
- [Installation](#installation)
- [Usage](#usage)
- [Issues](#issues)

## Introduction
ElephantChess is a Chinese Chess (Xiangqi) GUI application designed to play and analyze games, similar to how "Arena" functions for European Chess. Inspired by the commercial "Fritz" software, it's currently in a pre-alpha stage and features a basic AI opponent (weakbot).

- [Learn about Xiangqi on Wikipedia](https://en.wikipedia.org/wiki/Xiangqi)
- [Learn about European Chess on Wikipedia](https://en.wikipedia.org/wiki/Chess)

<div align="center">
<img src="Screenshot.png" width="800" height="600">
</div>

## Rules
Xiangqi is played on a 9x10 board divided by a "river", with pieces positioned on intersections. The game involves two players, Red and Black, each equipped with a set of distinct pieces: one General, two Advisors, two Elephants, two Horses, two Chariots, two Cannons, and five Soldiers.

### Objective
The primary goal is to checkmate the opponent's General.

### Gameplay Rules
- **General**: Moves one point orthogonally within the palace, a specific 3x3 region.
- **Advisor**: Moves one point diagonally, remaining within the palace.
- **Elephant**: Moves two points diagonally, cannot cross the river.
- **Horse**: Moves in an “L” shape, can be blocked by other pieces.
- **Chariot**: Moves any number of points along a row or column.
- **Cannon**: Moves like the Chariot but must jump over one piece to capture another.
- **Soldier**: Moves one point forward; after crossing the river, it can also move sideways.

Players take turns, with each move aiming to threaten the opponent's General. The game concludes by checkmate or stalemate, the latter occurring if a player cannot make a legal move and their General is not in check.

## Installation

### Using QtCreator
1. Download and install Qt6.x for Linux from [Qt's official site](https://www.qt.io/download-open-source).
2. Clone the repository:
   ```bash
   git clone https://github.com/global667/ElephantChess.git
   ```
3. Open CMakeLists.txt in QtCreator and run the project.

### Using Installation Package

Visit www.elephant-chess.com, download the zip file, extract it, and follow the installation instructions.

## Usage

(TBD - Include information on how to use ElephantChess, covering any initial setup steps, basic commands, and options for gameplay.)

## Issues

- Compatibility: Should also run on Windows and macOS.
- Testing: Primarily tested on Ubuntu/wsl.
- Feedback and contributions are welcome to enhance the application. Enjoy!
