#pragma once
#include "Item2.h"

class AssasinW : public Item
{
private:
	int count;
	int bonusAGL;                          //무기가 추가해주는 AGL양

public:
	AssasinW(int d = 0, int bAGL = 50);    // 초기 0개, 추가 AGL = 50

	string GetWname() const override;      // 아이템의 이름 반환

	void UseW(int& AGL) override;

	int GetWCount() const override;
};