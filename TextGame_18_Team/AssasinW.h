#pragma once
#include "Item2.h"

class AssasinW : public Item
{
private:
	int count;
	int bonusAGI;                          //무기가 추가해주는 AGI양

public:
	AssasinW(int d = 0, int bAGI = 50);    // 초기 0개, 추가 AGI = 50

	string GetName() const override;      // 아이템의 이름 반환

	void Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI) override;

	int GetCount() const override;
};