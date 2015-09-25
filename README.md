Minimax search and Alpha-beta pruning for game playing
========================================================

We develop a Minimax and alpha-beta search based intelligent agent for [Warfare game](https://github.com/saisrivatsan/intelligentGameAgents/blob/master/Game%20Description.pdf). We implement the above with GUI using Qt

Installation
-------------
1. Install Qt-C++ and the dependant libraries.
2. Download the game and Compile the game in Release Mode.

Game Play
---------
Select the board, agent for player 1 and player 2. If any of the player is an automated agent (Minimax or A-B pruning), a difficulty slider will be displayed. Set the required difficulty level (1-5). If both the agents are A-B pruning,the difficulty can be set upto level 7, but note that game play is slower for higher difficulty.
Default values are already set ( Board-Keren, Player1-Human, Player2-Human, Difficulty level-3). 
Click on Start Game to begin. If one or more players are Human, the game can be played by clicking the buttons on the grid.
There is a status label which displays whose turn it is or the status (Begin Game/Game over along with the winner).
The LCD score boards show the current scores at every instant during game play. 

Game Logs
---------
Additionally after every game, we store the game data in a text file at the location ./build-Warfare-Desktop_Qt_5_3_MSVC2013_OpenGL_64bit-Release.

Read ./Outputs/ReadMeforGameData.txt for further details on how to find the file and interpret the data.
There is a RunAllCombo button which automatically plays the game for all combinations of board, depth and and algorithm for automated agents and logs the information.


Others
-------
Project submitted as part of Artificial Intelligence Course (CS60045), IIT Kharagpur.

Tested and verified in Qt 5.3 on Windows 
