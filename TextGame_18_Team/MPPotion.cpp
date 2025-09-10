#include "MPPotion.h"
#include <iostream>
using namespace std;

/*
    MPPotion 클래스:
     MP(마나/마력)를 회복하는 포션 아이템.
     Item(추상 클래스)을 상속받아 구체적인 동작을 구현한다.
        count         : 포션 개수
        restoreAmount : 포션 1개당 회복량
*/

MPPotion::MPPotion(int c, int restore)
{
    count = c;           // 초기 개수
    restoreAmount = restore; // 포션 하나로 회복되는 MP량
}

string MPPotion::GetName() const
{
    // 아이템 이름 반환
    return "MP 포션";
}

void MPPotion::Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI)
{
    // 개수가 0 이하면 사용 불가
    if (count <= 0)
        return;

    int before = mp;      // 사용 전 MP 기록
    mp += restoreAmount;  // MP 회복
    if (mp > maxMp)       // 최대 MP를 넘지 않도록 제한
        mp = maxMp;

    count--; // 포션 1개 소모

    // 디버그용 출력 (전투 로직과 연동 시 확인 가능)
    // cout << "[MP 포션 사용] MP: " << before << " -> " << mp
    //      << " / 남은 포션: " << count << endl;
}

int MPPotion::GetCount() const
{
    // 남은 포션 개수 반환
    return count;
}
