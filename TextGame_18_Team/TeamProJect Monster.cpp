// TeamProJect Monster.cpp
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>

#include "Item2.h"
#include "HPPotion.h"
#include "MPPotion.h"

using namespace std;

class Monster {
protected:
    int Hp;
    int AttackPower;
    int AttackSpeed;   // 선공 판정
    int CriticalHit;   // 치명타 확률(%)
    std::vector<Item*> Drops; // 드랍 아이템(전투 종료 시 Inventory로 소유권 이전)

public:
    Monster(int Health, int Atk, int Spd, int Crit, std::vector<Item*> drop)
        : Hp(Health), AttackPower(Atk), AttackSpeed(Spd), CriticalHit(Crit), Drops(std::move(drop)) {
    }

    virtual ~Monster() {
        // 소유권 이전(TakeDrops) 안 하면 여기서 정리
        for (auto* p : Drops) delete p;
        Drops.clear();
    }

    // ── 이름 표시용 (요청: 각 몬스터의 클래스명)
    virtual const char* GetName() const { return "Monster"; }

    // 기본 정보
    int  GetHP() const { return Hp; }
    int  GetAttackPower() const { return AttackPower; }
    int  GetAttackSpeed() const { return AttackSpeed; }
    int  GetCriticalHit() const { return CriticalHit; }
    bool IsDead() const { return Hp <= 0; }

    // 피해/치명 계산
    void ApplyDamage(int raw) {
        int real = (raw < 1) ? 1 : raw;
        Hp = std::max(0, Hp - real);
    }
    int RollDamage() const {
        int chance = (CriticalHit < 0) ? 0 : (CriticalHit > 100 ? 100 : CriticalHit);
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> d(1, 100);
        bool crit = d(rng) <= chance;
        return crit ? (AttackPower * 2) : AttackPower;
    }

    // 스테이지 강화(스테이지는 GameManager가 관리)
    void ApplyStageLevel(int stageLevel) {
        Hp += stageLevel * 10;
        AttackPower += stageLevel * 3;
        AttackSpeed += stageLevel / 3; // 3스테이지당 +1
        CriticalHit = std::min(100, CriticalHit + stageLevel);
    }

    // 드랍 소유권 이전: Inventory로 넘길 때 호출
    std::vector<Item*> TakeDrops() {
        std::vector<Item*> out = std::move(Drops);
        Drops.clear();
        return out;
    }
};

// 파생 몬스터(드랍 = Item* 생성) + 이름 오버라이드
class Goblin : public Monster {
public:
    Goblin()
        : Monster(55, 10, 4, 5, std::vector<Item*>{ new HPPotion(1), new MPPotion(1) }) {
    }
    const char* GetName() const override { return "Goblin"; }
};
class Orc : public Monster {
public:
    Orc()
        : Monster(75, 13, 5, 7, std::vector<Item*>{ new HPPotion(1) }) {
    }
    const char* GetName() const override { return "Orc"; }
};
class Troll : public Monster {
public:
    Troll()
        : Monster(95, 15, 6, 8, std::vector<Item*>{ new HPPotion(2) }) {
    }
    const char* GetName() const override { return "Troll"; }
};
class Slime : public Monster {
public:
    Slime()
        : Monster(35, 7, 3, 3, std::vector<Item*>{ new HPPotion(1) }) {
    }
    const char* GetName() const override { return "Slime"; }
};

// 보스
class BossDragon : public Monster {
public:
    BossDragon()
        : Monster(160, 18, 7, 12, std::vector<Item*>{ new HPPotion(2), new MPPotion(2) }) {
    }
    const char* GetName() const override { return "BossDragon"; }
};

// 일반 몬스터 랜덤 소환
std::unique_ptr<Monster> SummonMonster() {
    std::random_device rd; std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 3);
    switch (dis(gen)) {
    case 0: return std::make_unique<Goblin>();
    case 1: return std::make_unique<Orc>();
    case 2: return std::make_unique<Troll>();
    case 3: return std::make_unique<Slime>();
    }
    return std::make_unique<Slime>();
}

// 스테이지 반영 일반 몬스터
std::unique_ptr<Monster> SummonMonster(int stageLevel) {
    auto m = SummonMonster();
    if (m) m->ApplyStageLevel(stageLevel);
    return m;
}

// 스테이지 반영 보스
std::unique_ptr<Monster> SummonBoss(int stageLevel) {
    auto b = std::make_unique<BossDragon>();
    b->ApplyStageLevel(stageLevel);
    return b;
}
