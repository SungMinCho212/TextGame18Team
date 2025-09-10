#include "Item2.h"
#include "HPPotion.h"
#include "MPPotion.h"
#include "AssasinW.h"
#include <iostream>
using namespace std;

/*
    Inventory 클래스는 플레이어의 아이템(포션 등)과 골드를 관리한다.
    items: Item*을 저장하는 벡터. (동적 할당된 아이템의 소유권을 가진다)
    gold: 보유 골드
	AddItem: 새로운 아이템을 인벤토리에 추가
	AddGold: 골드 추가
	GetGold: 현재 보유 골드 반환
	AutoUsePotions: 현재 HP/MP가 최대치의 절반 이하일 경우 자동으로 포션을 사용, 포션은 한 번만 사용하고 함수는 종료(return)한다


    포션 자동 사용: HP/MP가 절반 이하일 때 해당 포션을 자동으로 사용
    소멸자에서 items에 저장된 Item*를 delete 하여 메모리 누수를 막는다.
    Inventory는 포인터의 소유권을 가진다. 즉, AddItem으로 넘긴 포인터는 외부에서 delete하면 안 된다.
    현재 설계는 복사 생성자/대입 연산자가 기본 제공되므로 얕은 복사 문제가 생길 수 있다.
    복사 금지(delete) 처리하거나 스마트 포인터(std::unique_ptr)로 관리하는 것이 안전하다.
*/

Inventory::Inventory(void)
{
    gold = 0; // 생성 시 골드는 0으로 시작
}

Inventory::~Inventory(void)
{
    // 소멸자: 아이템 벡터의 모든 포인터를 delete
    for (auto item : items)
    {
        delete item; // Item 소멸자 호출 → 동적 메모리 해제
    }
    items.clear(); // 벡터 비우기
}

void Inventory::AddItem(Item* item)
{
    // 새로운 아이템을 인벤토리에 추가
    // 넘긴 포인터는 Inventory가 소유권을 가진다
    items.push_back(item);
}

void Inventory::AddGold(int amount)
{
    // 골드 추가
    gold += amount;
}

int Inventory::GetGold() const
{
    // 현재 보유 골드 반환
    return gold;
}
bool Inventory::SpendGold(int g)
{ 
    if (g < 0) return false; if (gold < g) return false; gold -= g; return true; 
}
/*
     현재 HP/MP가 최대치의 절반 이하일 경우 자동으로 포션을 사용, 포션은 한 번만 사용하고 함수는 종료(return)한다
     사용 후 아이템 개수가 0 이하라면 delete + 벡터에서 제거.
     검색은 순차적으로 진행하며 HP 조건을 먼저 검사 → HP 포션 우선 사용.
     즉, HP와 MP가 동시에 낮더라도 HP포션을 먼저 사용한다.
     아이템 식별은 문자열("HP", "MP") 검색으로 수행됨.

      
*/

void Inventory::AutoUsePotions(int& hp, int& mp, int maxHp, int maxMp, int& AGI)
{
    // HP가 절반 이하일 경우 HP포션 사용
    if (hp <= maxHp / 2)
    {
        for (size_t i = 0; i < items.size(); i++)
        {
            if (items[i]->GetName().find("HP") != string::npos)
            {
                items[i]->Use(hp, mp, maxHp, maxMp, AGI); // HP 회복
                if (items[i]->GetCount() <= 0)
                {
                    delete items[i];                 // 메모리 해제
                    items.erase(items.begin() + i);  // 벡터에서 제거
                }
                return; // HP포션 한 번 사용 후 종료
            }
        }
    }

    // MP가 절반 이하일 경우 MP포션 사용
    if (mp <= maxMp / 2)
    {
        for (size_t i = 0; i < items.size(); i++)
        {
            if (items[i]->GetName().find("MP") != string::npos)
            {
                items[i]->Use(hp, mp, maxHp, maxMp, AGI); // MP 회복
                if (items[i]->GetCount() <= 0)
                {
                    delete items[i];                 // 메모리 해제
                    items.erase(items.begin() + i);  // 벡터에서 제거
                }
                return; // MP포션 한 번 사용 후 종료
            }
        }
    }
}

/*
    PrintInventory:
    PrintInventory: 
    현재 인벤토리 상태를 콘솔에 출력.
    아이템 이름과 개수, 그리고 보유 골드를 보여준다.
*/
void Inventory::PrintInventory(void) const
{
    cout << "=== 인벤토리 목록 ===" << endl;
    for (size_t i = 0; i < items.size(); i++)
    {
        cout << i + 1 << ". " << items[i]->GetName()
            << " (" << items[i]->GetCount() << ")" << endl;
    }
    cout << "골드: " << gold << endl;
}

void Inventory::Clear() 
{
    for (auto* p : items) delete p;
    items.clear();
}

bool Inventory::UseByIndex(int idx, Character& ch) {
    if (idx < 0 || idx >= (int)items.size()) return false;
    Item* it = items[idx];
    if (!it) return false;

    // [CreatedByChatGPT] Character의 HP/MP를 로컬로 꺼내서 Item::Use에 전달
    int hp = ch.getHP();
    int mp = ch.getMP();        // (아래 B에서 추가할 getter)
    const int maxHp = ch.getMaxHP();
    const int maxMp = ch.getMaxMP(); // (아래 B에서 추가할 getter)
    int AGI = ch.getAGI();

    it->Use(hp, mp, maxHp, maxMp, AGI); // void 반환이 정상

    // [CreatedByChatGPT] 변경된 값 반영
    ch.setHP(hp);
    ch.setMP(mp);// (아래 B에서 추가할 setter)
    ch.setAGI(AGI);

    // 개수 0 이하이면 제거
    if (it->GetCount() <= 0) {
        delete it;
        items.erase(items.begin() + idx);
    }
    return true;
}