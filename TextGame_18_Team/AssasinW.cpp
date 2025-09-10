#include "AssasinW.h"
#include <iostream>
using namespace std;

AssasinW::AssasinW(int d, int bAGI)
{
    count = d;               // �ʱ� ����
    bonusAGI = bAGI;         // ������ �߰� �ɷ�ġ
}

string AssasinW::GetName() const
{
    // ������ �̸� ��ȯ
    return "������ ���̵�";
}

void AssasinW::Use (int& hp, int& mp, int maxHp, int maxMp, int& AGI)
{
    // count�� 0 �����̸� ��� �Ұ�
    if (count <= 0)
        return;

    int before = AGI; // ��� �� AGI ���
    AGI += bonusAGI; // AGI ����

    count--; // ���� 1�� �Ҹ�

}
int AssasinW::GetCount() const
{
    // ���� ���� ���� ��ȯ
    return count;
}
