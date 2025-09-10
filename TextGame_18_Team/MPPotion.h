#pragma once
#include "Item2.h"

class MPPotion : public Item
{
private:
    int count;
    int restoreAmount;

public:
    MPPotion(int c = 1, int restore = 30);

    string GetName() const override;

    void Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI) override;

    int GetCount() const override;
};
