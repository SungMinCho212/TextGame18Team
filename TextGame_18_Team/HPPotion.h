#pragma once
#include "Item2.h"

// HP 포션 클래스
// Item2.h에 정의된 Item(추상 클래스)을 상속받아 구현됨
class HPPotion : public Item
{
private:
    int count;       // 현재 소지한 포션 개수
    int healAmount;  // 포션 1개가 회복시켜주는 HP 양

public:
    // 생성자
    // c = 초기 포션 개수 (기본값: 1개)
    // heal = 회복량 (기본값: 30)
    HPPotion(int c = 1, int heal = 30);

    // 아이템의 이름 반환
    // "HP 포션"이라는 문자열을 반환하게 구현될 예정
    string GetName() const override;

    // 포션 사용 함수
    // hp, mp, maxHp, maxMp를 참조로 받아서 실제 HP를 회복시키는 역할
    void Use(int& hp, int& mp, int maxHp, int maxMp, int& AGI) override;

    // 남은 포션 개수 반환
    int GetCount() const override;
};
