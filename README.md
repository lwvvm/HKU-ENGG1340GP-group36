# MineHack
## ENGG1340 Gourp Project ---- Group36
### Team members:
- Liang Weibo
- Lin Yat Fei Angel
- Lin Zhongxi
- Lo Ka Lai
- Tso Kok Ching
- Wu Jun Hao

### application description:
- This is a game named "MineHack". Players need to avoid all mines which are randomly generated and find all safe places on a square map.
- Points system in this game: There are 4 different levels of difficulty for players to choose. Completing games of different difficulties can earn different scores. Completing more difficult games can earn more scores. Scores are used to buy items in the shop. There are several items in the shop that can help players complete the game more quickly and easily. When players complete a game quickly, they can earn extra points for their outstanding performance. Additionally, players can get points by answering the quiz which is about this lesson.
- Items include：
  - Invincibility: Being invincible in the following three steps
  - Autosweep: Show each cell of a 3*3 square
  - Shield: Players will not lose if they step on a mine for the first time in 30 seconds
- In each step, the player can choose 'r', 'f', 's' and 'i'.
  - 'r': Check a cell. If the cell is a mine, the player loses. Otherwise, the cell will show the number of mines in the adjacent cells.
  - 'f': Players can plant a flag on the cell  which he thinks is a mine.
  - 's': Players can enter the shop to buy items by points
  - 'i': Players use items to help themselves to complete the game

### features:
- After players choose the level of difficulty of the game, mines will be generated randomly on the map. (Generation of random events)
- After every step taken by players, the game needs to store the state of the game. (Data structures for storing data)
- After mines are generated randomly, the game stores the place of those mines to run the game. (Data structures for storing data)
- After players buy items in the shop, the game stores the number of the items that the player has. (Data structures for storing data)
- Players take steps and buy items in the shop changing the state of the game. (Dynamic memory management)
- Players' points change after winning the game and answer the quiz. (Dynamic memory management)
- The program includes the main.cpp, cover_display.cpp, item.cpp, guide.cpp, quiz_challenge.cpp and shop_menu.cpp. Codes in main.cpp involves other files of this game. (File input/output and program codes in multiple files)
- Players can choose the level of difficulty of the game. (Multiple difficulty levels)
- Different from traditional minesweeper games, this game has a shop and items to help players complete the game.
- The game also has a quiz about this lesson. Players can study while playing the game. 

### non-standard libraries:
- shop_menu.h
- Item.h
- quiz_challenge.h
- guide.h
- cover_display.h

### compliation/execution instrstruction:
- Download the zip, and throw it to your local device and type: 
  - make
  - ./game 
