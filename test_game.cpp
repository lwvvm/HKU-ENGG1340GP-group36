#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

// 定义角色类
class Character {
public:
    string name;
    string role;
    int max_hp;
    int hp;
    int attack;
    string ascii_art;

    Character(string n, string r, int h, int a, string art)
        : name(n), role(r), max_hp(h), hp(h), attack(a), ascii_art(art) {}

    bool isAlive() {
        return hp > 0;
    }

    void takeDamage(int dmg) {
        hp = max(0, hp - dmg);
    }
};

// 定义游戏类
class Game {
private:
    vector<Character> heroes;
    vector<Character> monsters;
    int currentHeroIndex;
    int currentMonsterIndex;
    int round;
    int actionPoints;
    int storedPoints;
    int allocatedAttack;
    int allocatedDefense;
    int allocatedStorage;
    bool hasSwitched;

    // 职业克制关系
    float getTypeMultiplier(string attackerRole, string defenderRole) {
        if ((attackerRole == "刺客" && defenderRole == "巫师") ||
            (attackerRole == "巫师" && defenderRole == "射手") ||
            (attackerRole == "射手" && defenderRole == "战士") ||
            (attackerRole == "战士" && defenderRole == "刺客")) {
            return 1.5;
        } else if ((defenderRole == "刺客" && attackerRole == "巫师") ||
                   (defenderRole == "巫师" && attackerRole == "射手") ||
                   (defenderRole == "射手" && attackerRole == "战士") ||
                   (defenderRole == "战士" && attackerRole == "刺客")) {
            return 0.5;
        }
        return 1.0;
    }

    // 打印角色信息
    void printCharacterInfo(Character &ch) {
        cout << ch.ascii_art << endl;
        cout << ch.name << " (" << ch.role << ") " << ch.hp << "/" << ch.max_hp << " HP, " << ch.attack << " ATK" << endl;
    }

    // 计算伤害
    int calculateDamage(Character &attacker, Character &defender, int attackPoints, int defensePoints) {
        if (attackPoints <= defensePoints) {
            return 0; // 如果攻击点数小于等于防御点数，不造成伤害
        }
        float typeMultiplier = getTypeMultiplier(attacker.role, defender.role);
        int effectivePoints = attackPoints - defensePoints;
        float pointMultiplier = 0.8 + 0.2 * effectivePoints;
        return static_cast<int>(attacker.attack * pointMultiplier * effectivePoints * typeMultiplier);
    }

    // 显示伤害预览
    void showDamagePreview(Character &attacker, Character &defender, int attackPoints, int defensePoints) {
        if (attackPoints == 0) {
            int previewDamage = calculateDamage(attacker, defender, 1, defensePoints);
            cout << "伤害预览: 1点攻击将造成 " << previewDamage << " 点伤害" << endl;
        } else {
            int currentDamage = calculateDamage(attacker, defender, attackPoints, defensePoints);
            int nextDamage = calculateDamage(attacker, defender, attackPoints + 1, defensePoints);
            cout << "当前伤害: " << currentDamage << "，如果再分配1点攻击，将造成 " << nextDamage << " 点伤害" << endl;
        }
    }

    // 英雄切换
    void switchHero() {
        cout << "请选择一个英雄进行切换：" << endl;
        for (int i = 0; i < heroes.size(); i++) {
            if (i == currentHeroIndex || !heroes[i].isAlive()) continue;
            cout << i + 1 << ". " << heroes[i].name << " (" << heroes[i].role << ") "
                 << heroes[i].hp << "/" << heroes[i].max_hp << " HP" << endl;
        }
        int choice;
        cin >> choice;
        currentHeroIndex = choice - 1;
        hasSwitched = true;
    }

    // 显示分配状态
    void showAllocation() {
        cout << "当前分配状态：" << endl;
        cout << "攻击: " << allocatedAttack << " 点" << endl;
        cout << "防御: " << allocatedDefense << " 点" << endl;
        cout << "储存: " << allocatedStorage << " 点" << endl;
        cout << "剩余行动点: " << actionPoints << endl;
        showDamagePreview(heroes[currentHeroIndex], monsters[currentMonsterIndex], allocatedAttack, 0);
    }

public:
    // 初始化游戏
    Game() {
        round = 1;
        actionPoints = 1;
        storedPoints = 0;
        allocatedAttack = 0;
        allocatedDefense = 0;
        allocatedStorage = 0;
        hasSwitched = false;

        // 初始化英雄
        heroes.push_back(Character("莱昂", "战士", 300, 60, "🛡️"));
        heroes.push_back(Character("哈娜", "巫师", 240, 80, "🔮"));
        heroes.push_back(Character("杰西", "射手", 220, 70, "🏹"));
        heroes.push_back(Character("赞恩", "刺客", 220, 70, "🗡️"));

        // 初始化魔物
        monsters.push_back(Character("无头骑士", "战士", 500, 40, "🪓"));
        monsters.push_back(Character("萨满", "巫师", 300, 80, "🌀"));
        monsters.push_back(Character("暗精灵", "射手", 280, 60, "🏹"));
        monsters.push_back(Character("蛛影潜行者", "刺客", 280, 60, "🕷️"));

        currentHeroIndex = 0;
        currentMonsterIndex = 0;
    }

    // 游戏主循环
    void play() {
        srand(time(0));

        while (true) {
            // 增加行动点，包含储存的行动点
            actionPoints = min(4, round) + storedPoints;
            storedPoints = 0; // 重置储存点
            allocatedAttack = 0;
            allocatedDefense = 0;
            allocatedStorage = 0;
            hasSwitched = false;

            // 打印回合信息
            cout << "========== 第 " << round << " 回合 ==========" << endl;
            cout << "行动点: " << actionPoints << " (储存: " << storedPoints << ")" << endl;

            // 打印当前英雄和魔物信息
            cout << "英雄：" << endl;
            printCharacterInfo(heroes[currentHeroIndex]);
            cout << "魔物：" << endl;
            printCharacterInfo(monsters[currentMonsterIndex]);

            while (actionPoints > 0) {
                cout << "选择操作: 1.储存, 2.防御, 3.攻击, 4.切换, 5.撤销, 6.投降" << endl;
                showAllocation();
                char action;
                cin >> action;

                if (action == '1') {
                    if (allocatedStorage < 4 && actionPoints > 0) {
                        allocatedStorage++;
                        actionPoints--;
                    } else {
                        cout << "无法储存更多行动点！" << endl;
                    }
                } else if (action == '2') {
                    if (actionPoints > 0) {
                        allocatedDefense++;
                        actionPoints--;
                    } else {
                        cout << "行动点不足！" << endl;
                    }
                } else if (action == '3') {
                    if (actionPoints > 0) {
                        allocatedAttack++;
                        actionPoints--;
                    } else {
                        cout << "行动点不足！" << endl;
                    }
                } else if (action == '4') {
                    if (!hasSwitched) {
                        switchHero();
                        actionPoints--;
                    } else {
                        cout << "本回合已切换过英雄！" << endl;
                    }
                } else if (action == '5') {
                    // 撤销操作
                    if (allocatedAttack > 0) {
                        allocatedAttack--;
                        actionPoints++;
                    } else if (allocatedDefense > 0) {
                        allocatedDefense--;
                        actionPoints++;
                    } else if (allocatedStorage > 0) {
                        allocatedStorage--;
                        actionPoints++;
                    } else {
                        cout << "没有可以撤销的操作！" << endl;
                    }
                } else if (action == '6') {
                    cout << "你选择了投降！游戏结束！" << endl;
                    return;
                }
            }

            // 如果攻击点数为0，则不允许执行攻击
            if (allocatedAttack == 0) {
                cout << "未分配攻击点，无法进行攻击！" << endl;
            } else {
                int damage = calculateDamage(heroes[currentHeroIndex], monsters[currentMonsterIndex], allocatedAttack, 0);
                monsters[currentMonsterIndex].takeDamage(damage);
                cout << "你对 " << monsters[currentMonsterIndex].name << " 造成了 " << damage << " 点伤害！" << endl;
            }

            // 检查魔物是否死亡
            if (!monsters[currentMonsterIndex].isAlive()) {
                cout << monsters[currentMonsterIndex].name << " 被击败了！" << endl;
                currentMonsterIndex++;
                if (currentMonsterIndex >= monsters.size()) {
                    cout << "恭喜你击败了所有魔物！你赢了！" << endl;
                    break;
                }
            }

            // 检查英雄是否死亡
            if (!heroes[currentHeroIndex].isAlive()) {
                cout << heroes[currentHeroIndex].name << " 被击败了！" << endl;
                switchHero();
                if (currentHeroIndex >= heroes.size()) {
                    cout << "所有英雄阵亡，游戏失败！" << endl;
                    break;
                }
            }

            // 储存点累加到下回合
            storedPoints = allocatedStorage;
            round++;
        }
    }
};

// 主函数
int main() {
    Game game;
    game.play();
    return 0;
}
