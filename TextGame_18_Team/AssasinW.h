#pragma once
#include "Item2.h"

class AssasinW : public Item
{
private:
	int count;
	int bonusAGL;                          //���Ⱑ �߰����ִ� AGL��

public:
	AssasinW(int d = 0, int bAGL = 50);    // �ʱ� 0��, �߰� AGL = 50

	string GetWname() const override;      // �������� �̸� ��ȯ

	void UseW(int& AGL) override;

	int GetWCount() const override;
};