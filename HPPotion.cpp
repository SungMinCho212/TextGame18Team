#include "HPPotion.h"
#include <iostream>
using namespace std;

/*
    HPPotion 클래스:
     HP를 회복하는 포션 아이템.
     Item(추상 클래스)을 상속받아 구체적으로 동작을 구현한다.
        count      : 포션 개수
        healAmount : 회복량
*/

HPPotion::HPPotion(int c, int heal)
{
    count = c;        // 초기 개수
    healAmount = heal; // 포션 1개당 회복량
}

string HPPotion::GetName() const
{
    // 아이템 이름 반환
    return "HP 포션";
}

void HPPotion::Use(int& hp, int& mp, int maxHp, int maxMp)
{
    // count가 0 이하이면 사용 불가
    if (count <= 0)
        return;

    int before = hp; // 사용 전 HP 기록
    hp += healAmount; // HP 회복
    if (hp > maxHp)   // 최대 HP를 넘지 않도록 제한
        hp = maxHp;

    count--; // 포션 1개 소모

    // 디버그용 출력 (전투 로직과 연동 시 확인 가능)
    // cout << "[HP 포션 사용] HP: " << before << " -> " << hp 
    //      << " / 남은 포션: " << count << endl;
}

int HPPotion::GetCount() const
{
    // 현재 포션 개수 반환
    return count;
}
