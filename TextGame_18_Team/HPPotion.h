#pragma once
#include "Item2.h"

// HP ���� Ŭ����
// Item2.h�� ���ǵ� Item(�߻� Ŭ����)�� ��ӹ޾� ������
class HPPotion : public Item
{
private:
    int count;       // ���� ������ ���� ����
    int healAmount;  // ���� 1���� ȸ�������ִ� HP ��

public:
    // ������
    // c = �ʱ� ���� ���� (�⺻��: 1��)
    // heal = ȸ���� (�⺻��: 30)
    HPPotion(int c = 1, int heal = 30);

    // �������� �̸� ��ȯ
    // "HP ����"�̶�� ���ڿ��� ��ȯ�ϰ� ������ ����
    string GetName() const override;

    // ���� ��� �Լ�
    // hp, mp, maxHp, maxMp�� ������ �޾Ƽ� ���� HP�� ȸ����Ű�� ����
    void Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI) override;

    // ���� ���� ���� ��ȯ
    int GetCount() const override;
};
