#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Character.cpp"
using namespace std;

// ������ �߻� Ŭ����
class Item
{
public:
    virtual ~Item() {}

    //���� �̸� ��ȯ
    virtual string GetWname() const = 0;

    //���� ���
    virtual void UseW(int& AGL) = 0;
    
    //����
    virtual int GetWCount() const = 0;

    // ������ �̸� ��ȯ
    virtual string GetName() const = 0;

    // ������ ��� (���߿� Character& player�� ��ü ����)
    virtual void Use(int& hp, int& mp, int maxHp, int maxMp) = 0;

    // ���� ���� ��ȯ
    virtual int GetCount() const = 0;
};

// �κ��丮 Ŭ����
class Inventory
{
private:
    vector<Item*> items; // HP/MP ���� �����۸� ����
    int gold;            // ������

public:
    Inventory(void);

    ~Inventory(void);

    // ������ �߰�
    void AddItem(Item* item);

    // ������ �߰�
    void AddGold(int amount);

    // ������ Ȯ��
    int GetGold() const;

    // HP/MP 50% ���� �ڵ� ���
    void AutoUsePotions(int& hp, int& mp, int maxHp, int maxMp);

    // �κ��丮 ���� ��� (���� �̸� + ����, ���)
    void PrintInventory(void) const;

    bool SpendGold(int g);
    void Clear();
    bool UseByIndex(int idx, Character& ch);
};
