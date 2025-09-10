#include "AssasinW.h"
#include <iostream>
using namespace std;

AssasinW::AssasinW(int d, int bAGL)
{
    count = d;               // �ʱ� ����
    bonusAGL = bAGL;         // ������ �߰� �ɷ�ġ
}

string AssasinW::GetWname() const
{
    // ������ �̸� ��ȯ
    return "������ ���̵�";
}

void AssasinW::UseW (int& AGL)
{
    // count�� 0 �����̸� ��� �Ұ�
    if (count <= 0)
        return;

    int before = AGL; // ��� �� AGL ���
    AGL += bonusAGL; // AGL ����

    count--; // ���� 1�� �Ҹ�

}
int AssasinW::GetWCount() const
{
    // ���� ���� ���� ��ȯ
    return count;
}
