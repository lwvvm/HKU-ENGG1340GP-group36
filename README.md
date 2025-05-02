# MineHack
## ENGG1340 Gourp Porjrct ---- Group36
### Team members:
- Liang Weibo
- Lin Yet Fei Angel
- Lin Zhongxi
- Lo Ka Lai
- Tso Kok Ching
- Wu Jun Hao

### application description:
- This is a game named "MineHack". Players need to avoid all mines which are randomly generated and find all safe places in a square map.
- Points system in this game: There are 4 different levels of difficulty for players to choose. Completing games of different difficulties can earn different scores. Obiviously, completing more difficult games can earn more scores. Scores are used to buy items in the shop. There are several items in the shop which can help players complete the game more quickly and easily. When players complete a game quickly, he can earn extra points for his outstanding performance. Additionally, players can get points by answering quiz which is about this lesson.
- items include：
  - Invincibility: Being invincible in the folllowing three steps
  - Autosweep: Show each cells of a 3*3 square
  - Shield: Players will not lose if they step on a mine for the first time in 30 seconds
- In each step, player can choose 'r', 'f', 's' and 'i'.
  - 'r': Check a cell. If the cell is a mine, the player loses. Otherwise, the cell will show the number of mines in the adjacent cells.
  - 'f': Players can plant a flag on the cell  which he think it is a mine.
  - 's': Players can enter the shop to buy items by points
  - 'i': Players use items to help themselves to comlpete the game

### features:
- There are different grade of difficulties for players to choose, making the game suitable for different playeres. The mines are generated randomly in each map, increasing the randomness and enjoyment of this game. PLayers can buy items in the game to help themselves which involves dynamic memory management, making the game unique and innovative. Besides, the little quiz in the game can help student review what they have learned in this lesson, so the game is good for students' study too.

### non-standard libraries:
- shop_menu.h
- Item.h
- quiz_challenge.h
- guide.h

### compliation/execution instrstruction:
- Download the zip, and throw it to your local device and type: 
  - make
  - ./game 
