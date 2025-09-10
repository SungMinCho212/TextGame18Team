#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <memory> 
#include <algorithm>

using namespace std;

class Monster 
{
protected:
    int Hp;
    int AttackPower;
    int AttackSpeed;   
    int CriticalHit;   
    vector<string> Items;

public:
    Monster(int Health, int AttackPower, int AttackSpeed, int CriticalHit, vector<string> DropItems)
        : Hp(Health), AttackPower(AttackPower), AttackSpeed(AttackSpeed), CriticalHit(CriticalHit), Items(DropItems) 
    {}

    virtual ~Monster() = default; 

    // 게터
    int GetHP() const { return Hp; }
    int GetAttackPower() const { return AttackPower; }
    int GetAttackSpeed() const { return AttackSpeed; }
    int GetCriticalHit() const { return CriticalHit; }
    const vector<string>& itemsRef() const { return Items; } 

    // 전투 유틸
    bool IsDead() const { return Hp <= 0; }

    void ApplyDamage(int raw) {
        int real = std::max(1, raw); 
        Hp = std::max(0, Hp - real);
    }

    int RollDamage() const 
    {
        int chance = (CriticalHit < 0) ? 0 : (CriticalHit > 100 ? 100 : CriticalHit);

        static std::mt19937 rng{ std::random_device{}() };

        std::uniform_int_distribution<int> d(1, 100);

        bool crit = d(rng) <= chance;

        return crit ? (AttackPower * 2) : AttackPower;
    }

    void ApplyStageLevel(int stageLevel) 
    {
        Hp += stageLevel * 10;
        AttackPower += stageLevel * 3;
        AttackSpeed += stageLevel / 3; 
        CriticalHit = std::min(100, CriticalHit + stageLevel);
    }
};


class Goblin : public Monster 
{
public:
    Goblin() : Monster(55, 10, 4, 5, { "거친가죽", "짧은단검" }) {}
};

class Orc : public Monster 
{
public:
    Orc() : Monster(75, 13, 5, 7, { "오크뼈", "부러진철퇴" }) {}
};

class Troll : public Monster 
{
public:
    Troll() : Monster(95, 15, 6, 8, { "트롤피", "무딘클럽" }) {}
};

class Slime : public Monster
{
public:
    Slime() : Monster(35, 7, 3, 3, { "젤리조각" }) {}
};


class BossDragon : public Monster 
{
public:
    BossDragon() : Monster(160, 18, 7, 12, { "용비늘", "용의심장" }) {}
    
};


std::unique_ptr<Monster> SummonMonster() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 3);
    switch (dis(gen)) {
    case 0: return std::make_unique<Goblin>();
    case 1: return std::make_unique<Orc>();
    case 2: return std::make_unique<Troll>();
    case 3: return std::make_unique<Slime>();
    }
    return std::make_unique<Slime>();
}


std::unique_ptr<Monster> SummonMonster(int stageLevel) 
{
    auto m = SummonMonster();
    if (m) m->ApplyStageLevel(stageLevel);
    return m;
}


std::unique_ptr<Monster> SummonBoss(int stageLevel) 
{
    auto b = std::make_unique<BossDragon>();
    b->ApplyStageLevel(stageLevel);
    return b;
}
