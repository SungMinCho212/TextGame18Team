#pragma once
#include "Item2.h"

class AssasinW : public Item
{
private:
	int count;
	int bonusAGI;                          //���Ⱑ �߰����ִ� AGI��

public:
	AssasinW(int d = 0, int bAGI = 50);    // �ʱ� 0��, �߰� AGI = 50

	string GetName() const override;      // �������� �̸� ��ȯ

	void Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI) override;

	int GetCount() const override;
};