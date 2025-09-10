#include "AssasinW.h"
#include <iostream>
using namespace std;

AssasinW::AssasinW(int d, int bAGI)
{
    count = d;               // 초기 개수
    bonusAGI = bAGI;         // 무가의 추가 능력치
}

string AssasinW::GetName() const
{
    // 아이템 이름 반환
    return "섀도우 블레이드";
}

void AssasinW::Use (int& hp, int& mp, int maxHp, int maxMp, int& AGI)
{
    // count가 0 이하이면 사용 불가
    if (count <= 0)
        return;

    int before = AGI; // 사용 전 AGI 기록
    AGI += bonusAGI; // AGI 증가

    count--; // 포션 1개 소모

}
int AssasinW::GetCount() const
{
    // 현재 무기 개수 반환
    return count;
}
