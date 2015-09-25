Naming Convention
-----------------
The text files are named using this convention
B_<board_name> Depth_<limiting Depth> <Agent1_Algorithm>vs<Agent2_Algorithm>

For example, if you want to search for a Minimax vs Alpha-beta Pruning game @ depth 4
played on board keren, search for the file
"B_Keren D_4 G_Minimax vs A-B Pruning.txt"

Outputs Available
-----------------
Boards : Keren, Narvik, Sevastopol, Smolensk, Westerplatte
Depth : 3,4,5 (Depth 6 and 7 available for Alpha-Beta pruning games only)
Agent Algorithms : Minimax vs Minimax, Alpha-beta pruning vs Alpha-beta pruning, Minimax vs Alpha-beta pruning, Alpha-beta pruning vs Minimax

Interpreting data stored in the files
--------------------------------------
The data in the files are organised as follows :

1. Game details - Board, Agent Algorithm and Depth
2. Move Sequence (Blue goes first. Same grid naming conventions as specified)
3. Final Board State ( B and G denote boxes occupied by Blue and Green respectively)
4. Scores and status of the game (Winner declared)
5. Nodes expanded at every move, total and average (Total Number of Nodes expanded, not number of the leaf nodes)
6. Time Elapse (Total and Average in ms)