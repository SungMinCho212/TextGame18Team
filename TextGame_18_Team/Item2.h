#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Character.cpp"
using namespace std;

// 아이템 추상 클래스
class Item
{
public:
    virtual ~Item() {}

    //무기 이름 반환
    virtual string GetWname() const = 0;

    //무기 사용
    virtual void UseW(int& AGL) = 0;
    
    //개수
    virtual int GetWCount() const = 0;

    // 아이템 이름 반환
    virtual string GetName() const = 0;

    // 아이템 사용 (나중에 Character& player로 교체 가능)
    virtual void Use(int& hp, int& mp, int maxHp, int maxMp) = 0;

    // 포션 갯수 반환
    virtual int GetCount() const = 0;
};

// 인벤토리 클래스
class Inventory
{
private:
    vector<Item*> items; // HP/MP 포션 아이템만 보관
    int gold;            // 소지금

public:
    Inventory(void);

    ~Inventory(void);

    // 아이템 추가
    void AddItem(Item* item);

    // 소지금 추가
    void AddGold(int amount);

    // 소지금 확인
    int GetGold() const;

    // HP/MP 50% 이하 자동 사용
    void AutoUsePotions(int& hp, int& mp, int maxHp, int maxMp);

    // 인벤토리 상태 출력 (포션 이름 + 개수, 골드)
    void PrintInventory(void) const;

    bool SpendGold(int g);
    void Clear();
    bool UseByIndex(int idx, Character& ch);
};
