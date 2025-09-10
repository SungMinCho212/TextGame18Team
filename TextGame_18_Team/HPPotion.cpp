#include "HPPotion.h"
#include <iostream>
using namespace std;

/*
    HPPotion Ŭ����:
     HP�� ȸ���ϴ� ���� ������.
     Item(�߻� Ŭ����)�� ��ӹ޾� ��ü������ ������ �����Ѵ�.
        count      : ���� ����
        healAmount : ȸ����
*/

HPPotion::HPPotion(int c, int heal)
{
    count = c;        // �ʱ� ����
    healAmount = heal; // ���� 1���� ȸ����
}

string HPPotion::GetName() const
{
    // ������ �̸� ��ȯ
    return "HP ����";
}

void HPPotion::Use(int& hp, int& mp, int maxHp, int maxMp)
{
    // count�� 0 �����̸� ��� �Ұ�
    if (count <= 0)
        return;

    int before = hp; // ��� �� HP ���
    hp += healAmount; // HP ȸ��
    if (hp > maxHp)   // �ִ� HP�� ���� �ʵ��� ����
        hp = maxHp;

    count--; // ���� 1�� �Ҹ�

    // ����׿� ��� (���� ������ ���� �� Ȯ�� ����)
    // cout << "[HP ���� ���] HP: " << before << " -> " << hp 
    //      << " / ���� ����: " << count << endl;
}

int HPPotion::GetCount() const
{
    // ���� ���� ���� ��ȯ
    return count;
}
