#ifndef SHOP_MENU_H
#define SHOP_MENU_H

void saveItems(int TemporaryInvincibility, int AutoSweep, int MineScanner, int ExtraHint);
void loadItems(int& TemporaryInvincibility, int& AutoSweep, int& MineScanner, int& ExtraHint);
void shop_menu(int& totalScore, int& TemporaryInvincibility, int& AutoSweep, int& MineScanner, int& ExtraHint);

#endif