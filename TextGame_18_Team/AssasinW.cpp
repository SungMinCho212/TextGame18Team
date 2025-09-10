#include "AssasinW.h"
#include <iostream>
using namespace std;

AssasinW::AssasinW(int d, int bAGL)
{
    count = d;               // 초기 개수
    bonusAGL = bAGL;         // 무가의 추가 능력치
}

string AssasinW::GetWname() const
{
    // 아이템 이름 반환
    return "섀도우 블레이드";
}

void AssasinW::UseW (int& AGL)
{
    // count가 0 이하이면 사용 불가
    if (count <= 0)
        return;

    int before = AGL; // 사용 전 AGL 기록
    AGL += bonusAGL; // AGL 증가

    count--; // 포션 1개 소모

}
int AssasinW::GetWCount() const
{
    // 현재 무기 개수 반환
    return count;
}
