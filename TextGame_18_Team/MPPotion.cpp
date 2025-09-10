#include "MPPotion.h"
#include <iostream>
using namespace std;

/*
    MPPotion Ŭ����:
     MP(����/����)�� ȸ���ϴ� ���� ������.
     Item(�߻� Ŭ����)�� ��ӹ޾� ��ü���� ������ �����Ѵ�.
        count         : ���� ����
        restoreAmount : ���� 1���� ȸ����
*/

MPPotion::MPPotion(int c, int restore)
{
    count = c;           // �ʱ� ����
    restoreAmount = restore; // ���� �ϳ��� ȸ���Ǵ� MP��
}

string MPPotion::GetName() const
{
    // ������ �̸� ��ȯ
    return "MP ����";
}

void MPPotion::Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI)
{
    // ������ 0 ���ϸ� ��� �Ұ�
    if (count <= 0)
        return;

    int before = mp;      // ��� �� MP ���
    mp += restoreAmount;  // MP ȸ��
    if (mp > maxMp)       // �ִ� MP�� ���� �ʵ��� ����
        mp = maxMp;

    count--; // ���� 1�� �Ҹ�

    // ����׿� ��� (���� ������ ���� �� Ȯ�� ����)
    // cout << "[MP ���� ���] MP: " << before << " -> " << mp
    //      << " / ���� ����: " << count << endl;
}

int MPPotion::GetCount() const
{
    // ���� ���� ���� ��ȯ
    return count;
}
